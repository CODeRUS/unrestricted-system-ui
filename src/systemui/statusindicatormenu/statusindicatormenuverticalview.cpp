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
#include <MSceneManager>
#include <MSeparator>
#include <MApplicationExtensionArea>
#include <MBasicListItem>
#include <MImageWidget>
#include <MStylableWidget>
#include <MPannableViewport>
#include <MPositionIndicator>
#include <QGraphicsLinearLayout>
#include <QGraphicsSceneMouseEvent>
#include <mscenewindowview.h>
#include "notificationarea.h"
#include "statusindicatormenu.h"
#include "statusindicatormenuverticalview.h"

StatusIndicatorMenuVerticalView::StatusIndicatorMenuVerticalView(StatusIndicatorMenu *controller) :
    MSceneWindowView(controller),
    controller(controller)
{
    // Create an extension area for the top row plugins
    MApplicationExtensionArea *extensionArea = new MApplicationExtensionArea("com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0");
    connect(extensionArea, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), controller, SLOT(setStatusIndicatorMenuInterface(MApplicationExtensionInterface*)));
    connect(extensionArea, SIGNAL(extensionInstantiated(MApplicationExtensionInterface*)), this, SLOT(setExtensionLayoutPosition(MApplicationExtensionInterface*)));
    extensionArea->setObjectName("StatusIndicatorMenuExtensionArea");
    setSafeMode(extensionArea, QFile(CRASH_FILE).exists());
    extensionArea->init();

    // Add panning to the expension area
    MPannableViewport* viewport = new MPannableViewport();
    viewport->setWidget(extensionArea);
    viewport->setVerticalPanningPolicy(MPannableWidget::PanningAsNeeded);
    viewport->setStyleName("StatusIndicatorMenuViewport");
    viewport->positionIndicator()->setStyleName("CommonPositionIndicatorInverted");

    // Put the extension area to a horizontal layout
    QGraphicsLinearLayout *vlayout = new QGraphicsLinearLayout(Qt::Vertical);
    vlayout->setContentsMargins(0, 0, 0, 0);
    vlayout->setSpacing(0);
    vlayout->addItem(viewport);

    // Add a separator line on the bottom of the menu
    MStylableWidget *bottomSeparator = new MStylableWidget;
    bottomSeparator->setStyleName("StatusIndicatorMenuBottomSeparator");
    bottomSeparator->setLayoutPosition(M::VerticalTopPosition);
    vlayout->addItem(bottomSeparator);

    // Create a container widget for extension area and settings button layout
    containerWidget = new MStylableWidget;
    containerWidget->setStyleName("StatusIndicatorMenuExtensionAreaWidget");
    containerWidget->setLayout(vlayout);

    QGraphicsLinearLayout *hlayout = new QGraphicsLinearLayout(Qt::Horizontal);
    hlayout->setContentsMargins(0, 0, 0, 0);
    hlayout->setSpacing(0);
    hlayout->addStretch();
    hlayout->addItem(containerWidget);
    hlayout->addStretch();

    // Add a separator line on the right of the menu
    MStylableWidget *rightSeparator = new MStylableWidget;
    rightSeparator->setStyleName("StatusIndicatorMenuRightSeparator");
    rightSeparator->setLayoutPosition(M::HorizontalLeftPosition);
    hlayout->addItem(rightSeparator);

    controller->setLayout(hlayout);
    containerWidget->installEventFilter(this);
}

bool StatusIndicatorMenuVerticalView::eventFilter(QObject *obj, QEvent *event)
{
    MStylableWidget *widget = dynamic_cast<MStylableWidget *> (obj);

    if (widget != NULL && (event->type() == QEvent::GraphicsSceneMousePress || event->type() == QEvent::GraphicsSceneMouseDoubleClick || event->type() == QEvent::GraphicsSceneMouseRelease)) {
        return true;
    } else {
        return QObject::eventFilter(obj, event);
    }
}

void StatusIndicatorMenuVerticalView::setExtensionLayoutPosition(MApplicationExtensionInterface *extension)
{
    MWidgetController *widget = dynamic_cast<MWidgetController*>(extension->widget());

    if (widget) {
        widget->setLayoutPosition(M::VerticalCenterPosition);
    }
}

StatusIndicatorMenuVerticalView::~StatusIndicatorMenuVerticalView()
{
    delete containerWidget;
}

void StatusIndicatorMenuVerticalView::setSafeMode(MApplicationExtensionArea *extensionArea, bool enabled)
{
    QStringList order;
    QRegExp filter;

    QFile configFile("/home/user/.status-menu/items-order.conf");

    if (enabled || !configFile.exists()) {
        filter.setPattern("/statusindicatormenu-(volume|call|internetconnection|bluetooth|dlna|presence|transfer).desktop$");
        order << "statusindicatormenu-volume.desktop"
              << "statusindicatormenu-call.desktop"
              << "statusindicatormenu-internetconnection.desktop"
              << "statusindicatormenu-bluetooth.desktop"
              << "statusindicatormenu-dlna.desktop"
              << "statusindicatormenu-presence.desktop"
              << "statusindicatormenu-transfer.desktop";
    } else if (configFile.exists()) {
        if (configFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            order = QString(configFile.readAll()).split("\n");
            configFile.close();

            QStringList pattern;
            foreach (QString string, order)
                if (string.length() > 0
                 && string.contains("statusindicatormenu-")
                 && !string.contains("safemode"))
                    pattern.append(string.remove("statusindicatormenu-").remove(".desktop"));

            filter.setPattern(QString("/statusindicatormenu-(%1).desktop$").arg(pattern.join("|")));
            pattern.clear();
        }
    }

    extensionArea->setInProcessFilter(filter);
    extensionArea->setOutOfProcessFilter(QRegExp("$^"));
    extensionArea->setOrder(order);
}

M_REGISTER_VIEW_NEW(StatusIndicatorMenuVerticalView, StatusIndicatorMenu)
