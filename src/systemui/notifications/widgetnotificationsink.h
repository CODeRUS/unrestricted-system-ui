/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef WIDGETNOTIFICATIONSINK_H
#define WIDGETNOTIFICATIONSINK_H

#include "notificationsink.h"
#include <MBanner>

class MGConfItem;

/*!
 * WidgetNotificationSink is a common base class for all notification sinks that trigger
 * visual feedback using a graphics widget.
 *
 * WidgetNotificationSink creates MBanner widgets for notifications. When MBanner
 * is clicked the action bound to the notification is triggered and notification is removed
 * from the notification system signaling notificationRemovalRequested().
 *
 * The sink can be configured to always show the given notification text in the
 * banners (the default mode) or to only show a generic text instead of the
 * given notification text if the privacy mode is enabled by setting the value
 * of the /desktop/meego/privacy/private_lockscreen_notifications GConf key to
 * true. This can be accomplished with the setHonorPrivacySetting() call.
 */
class WidgetNotificationSink : public NotificationSink
{
    Q_OBJECT

public:
    /*!
     * Constructs a widget notification sink.
     */
    WidgetNotificationSink();

    /*!
     * Controls whether the notification banners should only show a generic text
     * instead of the full notification text if the
     * /desktop/meego/privacy/private_lockscreen_notifications GConf key is
     * set to true. If the GConf key is not honored the private mode is never
     * used.
     *
     * \param honor if \c true, privacy setting is honored. If \c false, the privacy setting is ignored.
     */
    void setHonorPrivacySetting(bool honor);

    //! MBanner property to store the notification ID into
    static const char *NOTIFICATION_ID_PROPERTY;
    //! MBanner property to store the group ID into
    static const char *GROUP_ID_PROPERTY;
    //! MBanner property to store the user removability into
    static const char *USER_REMOVABLE_PROPERTY;

protected:
    /*!
     * Determines icon id of a notification based on the given notification parameters.
     * \param parameters Notification parameters to determine the icon id from.
     * \return Logical icon id as a string
     */
    static QString determineIconId(const NotificationParameters &parameters);

    /*!
     * Determines user removability of a notification based on the given notification parameters.
     * \param parameters Notification parameters to determine removability from.
     * \return \c true if the notification should be user removable, \c false otherwise
     */
    static bool determineUserRemovability(const NotificationParameters &parameters);

    /*!
     * Creates a MBanner widget to represent a notification object.
     * Ownership of the constructed object is passed to the caller.
     * \param notification The notification object to represent with the MBanner.
     * \return Constructed MBanner that represents the notification.
     */
    MBanner *createInfoBanner(const Notification &notification);

    /*!
     * Creates a MBanner widget from the given notification parameters.
     * Ownership of the constructed object is passed to the caller.
     * \param type Notification type on the basis of which info banner type is to be constructed.
     * \param groupId The group ID to be associated with the info banner.
     * \param params NotificationParameters according to which configure the MBanner.
     */
    MBanner *createInfoBanner(Notification::NotificationType type, uint groupId, const NotificationParameters &parameters);

    /*!
     * Removes old actions from the given info banner and adds the action
     * specified in the notification (if any)
     *
     * \param infoBanner the MBanner to update
     * \param parameters the NotificationParameters to get the action from
     */
    void updateActions(MBanner *infoBanner, const NotificationParameters &parameters);

    /*!
     * Creates a title text string from notification parameters.
     * \param parameters the NotificationParameters to get the title text from
     * \return the title text
     */
    static QString infoBannerTitleText(const NotificationParameters &parameters);

    /*!
     * Creates a subtitle text string from notification parameters.
     * \param parameters the NotificationParameters to get the subtitle text from
     * \return the subtitle text
     */
    static QString infoBannerSubtitleText(const NotificationParameters &parameters);

    /*!
     * Creates a generic text string from notification parameters.
     * \param parameters the NotificationParameters to get the generic text from
     * \return the generic text
     */
    static QString infoBannerGenericText(const NotificationParameters &parameters);

private slots:
    /*!
     * A slot for catching info banner clicks
     */
    void infoBannerClicked();

private:
    //! GConf key for enabling/disabling private notifications
    MGConfItem *privacySetting;
};

#endif // WIDGETNOTIFICATIONSINK_H
