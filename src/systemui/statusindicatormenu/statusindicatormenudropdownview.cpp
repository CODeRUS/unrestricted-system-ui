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
#include <QGraphicsSceneMouseEvent>
#include <MSceneManager>
#include <MSceneWindow>
#include <MApplicationExtensionArea>
#include <MStylableWidget>
#include <MEscapeButtonPanel>
#include <MButton>
#include <MOverlay>
#include <MPannableViewport>
#include <QGraphicsLinearLayout>
#include <MWidgetView>
#include "notificationarea.h"
#include <MViewCreator>
#include <QGraphicsAnchorLayout>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <MGConfItem>
#include "statusindicatormenu.h"
#include "statusindicatormenudropdownview.h"
#include "statusindicatormenustyle.h"
#include "sysuid.h"

const int StatusIndicatorMenuDropDownView::VIEW_INITIALIZATION_DELAY = 30 * 1000;


void EventEaterWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}

PannedWidgetController::PannedWidgetController(QGraphicsItem *parent) :
    MStylableWidget(parent),
    bottommostWidget_(NULL)
{
}

const QGraphicsWidget *PannedWidgetController::bottommostWidget() const
{
    return bottommostWidget_;
}

void PannedWidgetController::setBottommostWidget(const QGraphicsWidget *widget)
{
    bottommostWidget_ = widget;
}

void PannedWidgetController::setGeometry(const QRectF &rect)
{
    MWidgetController::setGeometry(rect);
    emit positionOrSizeChanged();
}

void PannedWidgetController::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->accept();
}

void PannedWidgetController::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (bottommostWidget_) {
        // Test that the y position of the event is bigger than the bottom edge of the bottommost widget.
        // The calculations are done in this widget's coordinate space.
        if (event->pos().y() > bottommostWidget_->y() + bottommostWidget_->geometry().height()) {
            emit pressedOutSideContents();
        }
    }
}

StatusIndicatorMenuDropDownView::StatusIndicatorMenuDropDownView(StatusIndicatorMenu *controller) :
    MSceneWindowView(controller),
    controller(controller),
    fixedPluginsExtensionArea(NULL),
    statusIndicatorExtensionArea(NULL),
    pannableViewport(NULL),
    closeButtonOverlay(NULL),
    backgroundWidget(new MStylableWidget)
{
    // Create close button overlay
    closeButtonOverlay = createCloseButtonOverlay();

    // Create the pannable area background widget
    backgroundWidget->setStyleName("StatusIndicatorMenuWindowBackground");

    connect(controller, SIGNAL(hideRequested()), this, SLOT(resetViewport()));
    connect(controller, SIGNAL(displayEntered()), SLOT(ensureIsViewable()));

    // Schedule the view to be initialized
    QTimer::singleShot(VIEW_INITIALIZATION_DELAY, this, SLOT(ensureIsViewable()));
}


StatusIndicatorMenuDropDownView::~StatusIndicatorMenuDropDownView()
{
    delete backgroundWidget;
    delete closeButtonOverlay;
    delete pannableViewport;
    delete topRowWidget;
}

QGraphicsWidget* StatusIndicatorMenuDropDownView::createTopRow()
{
    // Create an extension area for the fixed area plugins
    fixedPluginsExtensionArea = new MApplicationExtensionArea("com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0");
    connect(fixedPluginsExtensionArea, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), controller, SLOT(setStatusIndicatorMenuInterface(MApplicationExtensionInterface*)));
    fixedPluginsExtensionArea->setObjectName("StatusIndicatorMenuExtensionArea");

    QStringList order;
    QRegExp filter;

    QFile file("/home/user/.status-menu/top-order.conf");
    if (file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        order = QString(file.readAll()).split("\n");
        file.close();

        QStringList pattern;
        foreach (QString string, order)
            if (string.length() > 0
             && string.contains("statusindicatormenu-")
             && !string.contains("safemode"))
                pattern.append(string.remove("statusindicatormenu-").remove(".desktop"));

        filter.setPattern(QString("/statusindicatormenu-(%1).desktop$").arg(pattern.join("|")));
        pattern.clear();
    }
    else if (QFile(CRASH_FILE).exists())
    {
        filter.setPattern("/statusindicatormenu-(volume|safemode|call).desktop$");
        order << "statusindicatormenu-volume.desktop"
              << "statusindicatormenu-safemode.desktop"
              << "statusindicatormenu-call.desktop";
    }
    else
    {
        filter.setPattern("/statusindicatormenu-(volume|call).desktop$");
        order << "statusindicatormenu-volume.desktop"
              << "statusindicatormenu-call.desktop";
    }

    fixedPluginsExtensionArea->setInProcessFilter(filter);
    fixedPluginsExtensionArea->setOutOfProcessFilter(QRegExp("$^"));
    fixedPluginsExtensionArea->setOrder(order);

    // Put the extension area and the settings button to a horizontal layout
    QGraphicsLinearLayout *topRowLayout = new QGraphicsLinearLayout(Qt::Vertical);
    topRowLayout->setContentsMargins(0, 0, 0, 0);
    topRowLayout->setSpacing(0);
    topRowLayout->addItem(fixedPluginsExtensionArea);

    // Create a container widget for extension area and settings button layout
    MWidgetController *topRowWidget = new MStylableWidget;
    topRowWidget->setStyleName("StatusIndicatorMenuExtensionAreaWidget");
    topRowWidget->setLayout(topRowLayout);

    return topRowWidget;
}

MApplicationExtensionArea* StatusIndicatorMenuDropDownView::createVerticalExtensionArea()
{
    // Create an extension area for the pannable area plugins
    MApplicationExtensionArea *extensionArea = new MApplicationExtensionArea("com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0");
    extensionArea->setObjectName("StatusIndicatorMenuVerticalExtensionArea");

    QStringList order;
    QRegExp filter;

    QFile file("/home/user/.status-menu/pannable-order.conf");
    if (!QFile(CRASH_FILE).exists() && file.exists() && file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        order = QString(file.readAll()).split("\n");
        file.close();

        QStringList pattern;
        foreach (QString string, order)
            if (string.length() > 0
             && string.contains("statusindicatormenu-")
             && !string.contains("safemode"))
                pattern.append(string.remove("statusindicatormenu-").remove(".desktop"));

        filter.setPattern(QString("/statusindicatormenu-(%1).desktop$").arg(pattern.join("|")));
        pattern.clear();
    }
    else
    {
        filter.setPattern("/statusindicatormenu-(internetconnection|bluetooth|dlna|presence|transfer).desktop$");
        order << "statusindicatormenu-internetconnection.desktop"
              << "statusindicatormenu-bluetooth.desktop"
              << "statusindicatormenu-dlna.desktop"
              << "statusindicatormenu-presence.desktop"
              << "statusindicatormenu-transfer.desktop";
    }

    extensionArea->setInProcessFilter(filter);
    extensionArea->setOutOfProcessFilter(QRegExp("$^"));
    extensionArea->setOrder(order);
    connect(extensionArea,
        SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)),
        controller,
        SLOT(setStatusIndicatorMenuInterface(MApplicationExtensionInterface*)));
    return extensionArea;
}

void StatusIndicatorMenuDropDownView::setSafeMode(MApplicationExtensionArea *extensionArea, bool enabled)
{
}

MPannableViewport* StatusIndicatorMenuDropDownView::createPannableArea()
{
    // Create pannable area contents
    statusIndicatorExtensionArea = createVerticalExtensionArea();

    QGraphicsLinearLayout *contentLayout = new QGraphicsLinearLayout(Qt::Vertical);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    contentLayout->addItem(statusIndicatorExtensionArea);

    MGConfItem *displayNotifications = new MGConfItem("/desktop/meego/status_menu/display_notifications", this);

    if(style()->notificationArea() && displayNotifications->value(true).toBool()) {
        NotificationArea *notificationArea = new NotificationArea;
        notificationArea->setNotificationManagerInterface(Sysuid::instance()->notificationManagerInterface());
        connect(notificationArea, SIGNAL(bannerClicked()), controller, SIGNAL(hideRequested()));
        contentLayout->addItem(notificationArea);
    }

    MWidgetController *contentWidget = new MStylableWidget;
    contentWidget->setStyleName("StatusIndicatorMenuExtensionAreaWidget");
    contentWidget->setLayout(contentLayout);

    QGraphicsLinearLayout *pannableLayout = new QGraphicsLinearLayout(Qt::Vertical);
    pannableLayout->setContentsMargins(0, 0, 0, 0);
    pannableLayout->setSpacing(0);
    pannableLayout->addItem(contentWidget);
    //QGraphicsWidget *closeButtonRow = createCloseButtonRow();
    //pannableLayout->addItem(closeButtonRow);
    pannableLayout->addStretch();

    // Create a container widget for the pannable area
    PannedWidgetController *pannedWidget = new PannedWidgetController;
    pannedWidget->setLayout(pannableLayout);
    //pannedWidget->setBottommostWidget(closeButtonRow);
    pannedWidget->setBottommostWidget(contentWidget);
    connect(pannedWidget, SIGNAL(positionOrSizeChanged()), this, SLOT(setPannabilityAndLayout()));
    connect(pannedWidget, SIGNAL(pressedOutSideContents()), controller, SIGNAL(hideRequested()));

    // Setup the pannable viewport
    MPannableViewport *pannableViewport = new MPannableViewport;
    pannableViewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pannableViewport->setVerticalPanningPolicy(MPannableWidget::PanningAsNeeded);
    pannableViewport->setWidget(pannedWidget);
    return pannableViewport;
}

QGraphicsWidget* StatusIndicatorMenuDropDownView::createCloseButtonRow()
{
    // Create a close button for the pannable area
    MButton *closeButton = new MButton;
    closeButton->setViewType("icon");
    closeButton->setObjectName("StatusIndicatorMenuCloseButton");
    closeButton->setIconID("icon-m-framework-close-thumbnail");
    connect(closeButton, SIGNAL(clicked()), controller, SIGNAL(hideRequested()));

    // Add two overlay widgets that will not allow mouse events to pass through them next to the close button
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(new EventEaterWidget);
    layout->addItem(closeButton);
    layout->addItem(new EventEaterWidget);

    // Create the area itself
    MWidgetController *closeButtonArea = new MStylableWidget;
    closeButtonArea->setStyleName("CloseButtonArea");
    closeButtonArea->setLayout(layout);

    return closeButtonArea;
}

MOverlay *StatusIndicatorMenuDropDownView::createCloseButtonOverlay()
{
    // Create a close button
    MButton *closeButton = new MButton;
    closeButton->setViewType("icon");
    closeButton->setObjectName("StatusIndicatorMenuCloseButton");
    closeButton->setIconID("icon-m-framework-close-thumbnail");
    connect(closeButton, SIGNAL(clicked()), controller, SIGNAL(hideRequested()));

    // Add two overlay widgets that will not allow mouse events to pass through them next to the close button
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addItem(new EventEaterWidget);
    layout->addItem(closeButton);
    layout->addItem(new EventEaterWidget);

    // Create the overlay itself
    MOverlay *closeButtonOverlay = new MOverlay;
    closeButtonOverlay->setLayout(layout);
    closeButtonOverlay->setObjectName("CloseButtonOverlay");

    return closeButtonOverlay;
}

void StatusIndicatorMenuDropDownView::ensureIsViewable()
{
    fixedPluginsExtensionArea->init();

    if (statusIndicatorExtensionArea) {
        statusIndicatorExtensionArea->init();
    }
}

void StatusIndicatorMenuDropDownView::setPannabilityAndLayout()
{
    // Appear or disappear the close button overlay based on close area position
    const QGraphicsWidget *m_bottommostWidget = static_cast<PannedWidgetController *>(pannableViewport->widget())->bottommostWidget();
    qreal closeButtonRowBottomYPos = m_bottommostWidget->mapToItem(controller, QPointF(0, m_bottommostWidget->geometry().height())).y();
    if (controller->sceneManager()) {
        qreal screenHeight = controller->sceneManager()->visibleSceneSize().height();
        if (closeButtonRowBottomYPos <= screenHeight) {
            controller->sceneManager()->disappearSceneWindowNow(closeButtonOverlay);
        } else {
            controller->sceneManager()->appearSceneWindowNow(closeButtonOverlay);
        }
    }

    // Make the pannable area background window extend from the top of the pannable viewport halfway to the bottom of the close button row
    qreal viewPortYPos = pannableViewport->mapToItem(controller, QPointF()).y();
    qreal backgroundHeight = (closeButtonRowBottomYPos - viewPortYPos) / 2;
    if (backgroundHeight < 0) {
        backgroundHeight = 0;
    }
    backgroundWidget->setMinimumHeight(backgroundHeight);
    backgroundWidget->setMaximumHeight(backgroundHeight);
}

void StatusIndicatorMenuDropDownView::resetViewport()
{
    //pannableViewport->setPosition(QPointF(0,0));
}

void StatusIndicatorMenuDropDownView::applyStyle()
{
    MSceneWindowView::applyStyle();

    if (pannableViewport == NULL) {
        QGraphicsAnchorLayout *backgroundLayout = new QGraphicsAnchorLayout;
        backgroundLayout->setContentsMargins(0, 0, 0, 0);
        backgroundLayout->setSpacing(0);
        backgroundLayout->addCornerAnchors(backgroundWidget, Qt::TopLeftCorner, backgroundLayout, Qt::TopLeftCorner);
        backgroundLayout->setMaximumHeight(0);

        // Put all the stuff into the scene window layout
        pannableViewport = createPannableArea();
        topRowWidget = createTopRow();
        QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addItem(topRowWidget);
        layout->addItem(backgroundLayout);
        layout->addItem(pannableViewport);
        controller->setLayout(layout);
    }
}

M_REGISTER_VIEW_NEW(StatusIndicatorMenuDropDownView, StatusIndicatorMenu)
