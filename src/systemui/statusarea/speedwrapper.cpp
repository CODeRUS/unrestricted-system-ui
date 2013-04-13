#include "speedwrapper.h"

SpeedWrapper::SpeedWrapper(QObject *parent) :
    QObject(parent),
    m_tx_speed(0.0),
    m_rx_speed(0.0),
    m_bytes_data(0,0,0,0),
    m_is_enabled(true),
    m_speed_interface("wlan0"),
    m_update_time(1000),
    m_network_interface_valid(false),
    m_isOnline(false)
{
    m_gconf_enabled = new MGConfItem("/desktop/meego/status_area/display_netspeed",this);
    m_gconf_speed_interface = new MGConfItem("/desktop/meego/status_area/netspeed_interface",this);
    m_gconf_update_time = new MGConfItem("/desktop/meego/status_area/netspeed_update_time",this);
    m_gconf_when_online = new MGConfItem("/desktop/meego/status_area/display_netspeed_whenOnline",this);
    m_network_conf_manager = new QNetworkConfigurationManager(this);

    m_timer = new QTimer(this);

    connect(m_timer,SIGNAL(timeout()),SLOT(updateData()));
    connect(m_gconf_enabled,SIGNAL(valueChanged()),SLOT(gconf_value_changed()));
    connect(m_gconf_speed_interface,SIGNAL(valueChanged()),SLOT(gconf_value_changed()));
    connect(m_gconf_update_time,SIGNAL(valueChanged()),SLOT(gconf_value_changed()));
    connect(m_gconf_when_online,SIGNAL(valueChanged()),SLOT(gconf_value_changed()));
    connect(m_network_conf_manager,SIGNAL(onlineStateChanged(bool)),SLOT(onlineUpdate(bool)));

    m_network_conf_manager->updateConfigurations();
    gconf_value_changed();
}

SpeedWrapper::~SpeedWrapper()
{

}

void SpeedWrapper::updateData()
{
    bool ok;
    rxtxBytes bytes = getIfaceStat(m_speed_interface,&ok);
    if(ok)
    {
        //ok
        m_bytes_data.oldRxBytes = m_bytes_data.newRxBytes;
        m_bytes_data.oldTxBytes = m_bytes_data.newTxBytes;

        m_bytes_data.newRxBytes = bytes.rxBytes;
        m_bytes_data.newTxBytes = bytes.txBytes;

        //TODO: Values changed?

        float update_time = m_update_time;

        m_rx_speed = ((m_bytes_data.newRxBytes - m_bytes_data.oldRxBytes) / (update_time / 1000));
        m_tx_speed = ((m_bytes_data.newTxBytes - m_bytes_data.oldTxBytes) / (update_time / 1000));

        emit dataChanged();
    }
}

float SpeedWrapper::getRxSpeed() const
{
    return m_rx_speed;
}

float SpeedWrapper::getTxSpeed() const
{
    return m_tx_speed;
}

bool SpeedWrapper::isOnline() const
{
    return m_isOnline;
}
void SpeedWrapper::gconf_value_changed()
{
    bool ok;
    //check netspeed enabled
    if(!m_gconf_enabled->value().isValid())
        m_gconf_enabled->set(true);
    //check netspeed interface
    if(!m_gconf_speed_interface->value().isValid())
        m_gconf_speed_interface->set("wlan0");
    //check netspeed update time
    if(!m_gconf_update_time->value().isValid())
        m_gconf_update_time->set(1000);
    //check when online
    if(!m_gconf_when_online->value().isValid())
        m_gconf_when_online->set(false);


    m_is_enabled = m_gconf_enabled->value().toBool();
    m_speed_interface = m_gconf_speed_interface->value().toString();
    m_when_online = m_gconf_when_online->value().toBool();

    if(m_speed_interface.isEmpty() || m_speed_interface.isNull())
        m_speed_interface = "wlan0";
    m_update_time = m_gconf_update_time->value().toInt(&ok);
    if(!ok)
        m_update_time = 1000;

    QNetworkInterface interface = QNetworkInterface::interfaceFromName(m_speed_interface);
    m_network_interface_valid = interface.isValid();
    bool online = (interface.flags().testFlag(QNetworkInterface::IsUp) &&
                  interface.flags().testFlag(QNetworkInterface::IsRunning));
    if(online != m_isOnline)
    {
        emit onlineChanged(online);
        updateData();
    }
    m_isOnline = online;

    if(m_is_enabled)
    {
        if(m_when_online)
        {
            if(m_isOnline)
            {
                m_timer->stop();
                m_timer->start(m_update_time);
            }else{
                m_timer->stop();
            }
        }else{
            m_timer->stop();
            m_timer->start(m_update_time);
        }
    }else{
        m_timer->stop();
    }
}

QString SpeedWrapper::getTxSpeedStr()
{
    return fmtWrapper(m_tx_speed);
}

QString SpeedWrapper::getRxSpeedStr()
{
    return fmtWrapper(m_rx_speed);
}

QString SpeedWrapper::fmtWrapper(float in)
{
    QString wrapperStr;
    if((in / 1024) <= 1)
    {
      wrapperStr.sprintf("%.1fK",in / 1024);
    }
    else
    {
      if((in / (MByte)) <= 1)
        wrapperStr.sprintf("%.1fK",in / 1024);
      else
        wrapperStr.sprintf("%.1fM",in / (MByte));
    }
    if((m_network_interface_valid) && (in > 0))
        return wrapperStr;
    else
        return QString("-");
}

void SpeedWrapper::onlineUpdate(bool)
{
    m_network_conf_manager->updateConfigurations();
    gconf_value_changed();
}

rxtxBytes SpeedWrapper::getIfaceStat(QString ifaceName, bool *ok)
{
    QFile file(StatFile);
    int pos = 0;
    QString rx;
    QString tx;
    rxtxBytes bytes;
    *ok = false;

    if(!file.open(QIODevice::ReadOnly))
        return bytes;
    QTextStream stream(&file);
    QString s = stream.readAll();
    file.close();

    QRegExp ifaceExp = QRegExp("(\\w+:)");
    QRegExp rxExp = QRegExp("\\d+");
    QRegExp passExp = QRegExp("(\\W+\\d+){7}");

    while((pos = ifaceExp.indexIn(s,pos)) != -1)
    {
        pos += ifaceExp.matchedLength();
        QString iface =  ifaceExp.cap();
        if(iface == ifaceName + ":")
        {
            pos = rxExp.indexIn(s,pos);
            rx = rxExp.cap();
            pos += rxExp.matchedLength();
            pos = passExp.indexIn(s,pos);
            pos += passExp.matchedLength();
            rxExp.indexIn(s,pos);
            tx = rxExp.cap();
        }
    }

    if((rx.isEmpty()) || (tx.isEmpty()))
    {
        *ok = false;
        return bytes;
    }
    bytes.rxBytes = strtoull(qPrintable(rx),0,10);
    bytes.txBytes = strtoull(qPrintable(tx),0,10);
    if((bytes.rxBytes == ULLONG_MAX) || bytes.txBytes == ULLONG_MAX)
    {
        *ok = false;
        return bytes;
    }
    *ok = true;
    return bytes;
}
