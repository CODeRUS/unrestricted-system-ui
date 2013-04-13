#ifndef SPEEDWRAPPER_H
#define SPEEDWRAPPER_H

#ifndef UNIT_TEST

#include <QObject>
#include <QTimer>
#include <QFile>
#include <MGConfItem>
#include <QtNetwork/QNetworkInterface>
#include <QtNetwork/QNetworkConfigurationManager>

#define MByte 1024*1024
#define StatFile "/proc/net/dev"

struct bytes_data
{
    float oldRxBytes;
    float oldTxBytes;
    float newTxBytes;
    float newRxBytes;
    bytes_data(unsigned long long oldRx,unsigned long long oldTx,unsigned long long newRx,unsigned long long newTx)
        : oldRxBytes(oldRx),
          oldTxBytes(oldTx),
          newRxBytes(newRx),
          newTxBytes(newTx)
    {}
    bytes_data(){}
};

typedef struct
{
    unsigned long long rxBytes;
    unsigned long long txBytes;
} rxtxBytes;

class SpeedWrapper : public QObject
{
    Q_OBJECT
public:
    explicit SpeedWrapper(QObject *parent = 0);
    virtual ~SpeedWrapper();
    float getTxSpeed() const;
    float getRxSpeed() const;
    QString getTxSpeedStr();
    QString getRxSpeedStr();
    QString fmtWrapper(float);
    bool isOnline() const;
signals:
    void dataChanged();
    void onlineChanged(bool);
private slots:
    void updateData();
    void gconf_value_changed();
    void onlineUpdate(bool);
    rxtxBytes getIfaceStat(QString ifaceName,bool *ok);
private:
    QTimer *m_timer;
    float m_tx_speed;
    float m_rx_speed;
    struct bytes_data m_bytes_data;
    MGConfItem *m_gconf_enabled;
    MGConfItem *m_gconf_speed_interface;
    MGConfItem *m_gconf_update_time;
    MGConfItem *m_gconf_when_online;
    bool m_is_enabled;
    bool m_network_interface_valid;
    bool m_when_online;
    bool m_isOnline;
    int m_update_time;
    QString m_speed_interface;
    QNetworkConfigurationManager *m_network_conf_manager;
};
#endif

#endif // SPEEDWRAPPER_H
