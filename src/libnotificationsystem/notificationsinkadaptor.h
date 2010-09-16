/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp notificationsink.xml -a notificationsinkadaptor -c NotificationSinkAdaptor -l NotificationSink -i notificationsink.h -i metatypedeclarations.h
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef NOTIFICATIONSINKADAPTOR_H_1284454167
#define NOTIFICATIONSINKADAPTOR_H_1284454167

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include "notificationsink.h"
#include "metatypedeclarations.h"
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface com.meego.core.MNotificationSink
 */
class NotificationSinkAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.meego.core.MNotificationSink")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.meego.core.MNotificationSink\">\n"
"    <method name=\"addNotification\">\n"
"      <arg direction=\"in\" type=\"(uiuuia{sv})\" name=\"notification\"/>\n"
"      <annotation value=\"Notification\" name=\"com.trolltech.QtDBus.QtTypeName.In0\"/>\n"
"    </method>\n"
"    <method name=\"removeNotification\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"notificationId\"/>\n"
"    </method>\n"
"    <method name=\"addGroup\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"groupId\"/>\n"
"      <arg direction=\"in\" type=\"a{sv}\" name=\"parameters\"/>\n"
"      <annotation value=\"NotificationParameters\" name=\"com.trolltech.QtDBus.QtTypeName.In1\"/>\n"
"    </method>\n"
"    <method name=\"removeGroup\">\n"
"      <arg direction=\"in\" type=\"u\" name=\"groupId\"/>\n"
"    </method>\n"
"    <signal name=\"notificationRemovalRequested\">\n"
"      <arg type=\"u\" name=\"notificationId\"/>\n"
"    </signal>\n"
"    <signal name=\"notificationGroupClearingRequested\">\n"
"      <arg type=\"u\" name=\"groupId\"/>\n"
"    </signal>\n"
"  </interface>\n"
        "")
public:
    NotificationSinkAdaptor(NotificationSink *parent);
    virtual ~NotificationSinkAdaptor();

    inline NotificationSink *parent() const
    { return static_cast<NotificationSink *>(QObject::parent()); }

public: // PROPERTIES
public Q_SLOTS: // METHODS
    void addGroup(uint groupId, NotificationParameters parameters);
    void addNotification(Notification notification);
    void removeGroup(uint groupId);
    void removeNotification(uint notificationId);
Q_SIGNALS: // SIGNALS
    void notificationGroupClearingRequested(uint groupId);
    void notificationRemovalRequested(uint notificationId);
};

#endif
