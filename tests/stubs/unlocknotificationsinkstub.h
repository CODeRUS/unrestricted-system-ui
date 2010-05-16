#ifndef UNLOCKNOTIFICATIONSINKSTUB_H
#define UNLOCKNOTIFICATIONSINKSTUB_H

#include <unlocknotificationsink.h>

UnlockNotificationSink::UnlockNotificationSink ()
{

}

void
UnlockNotificationSink::addNotification(const Notification &notification)
{
    Q_UNUSED (notification);
}

void
UnlockNotificationSink::removeNotification(uint notificationId)
{
    Q_UNUSED (notificationId);
}

void
UnlockNotificationSink::setLockedState(bool islocked)
{
    Q_UNUSED (islocked);
}

void
UnlockNotifications::updateMissedEvents(int a, int b, int c, int d)
{
    Q_UNUSED (a);
    Q_UNUSED (b);
    Q_UNUSED (c);
    Q_UNUSED (d);
}

#endif
