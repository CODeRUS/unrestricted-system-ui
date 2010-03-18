/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of system ui.
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

#ifndef UT_NOTIFICATIONAREA_H
#define UT_NOTIFICATIONAREA_H

#include <QtTest/QtTest>
#include <QObject>
#include "notificationareamodel.h"

class DuiApplication;
class NotificationArea;
class DuiInfoBanner;
class QGraphicsLayoutItem;

class Ut_NotificationArea : public QObject
{
    Q_OBJECT

private slots:
    // Called before the first testfunction is executed
    void initTestCase();
    // Called after the last testfunction was executed
    void cleanupTestCase();
    // Called before each testfunction is executed
    void init();
    // Called after every testfunction
    void cleanup();

    // Test adding a notification
    void testAddNotification();
    // Test removing a notification
    void testRemoveNotification();
    // Test when a notification is added, then latest comes first.
    void testAddNotificationLatestComesFirst();
    // Test when a notification is updated then that banner comes first
    void testUpdatedNotificationComesFirst();

signals:
    void addNotification(DuiInfoBanner &notification);
    void removeNotification(DuiInfoBanner &notification);
    void notificationUpdated(DuiInfoBanner &notification);

public:
    static QGraphicsLayoutItem *addedItem;
    static const QGraphicsLayoutItem *removedItem;
    static int itemCount;

private:
    DuiApplication *app;

    // The object being tested
    NotificationArea *m_subject;
};

#endif
