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

#ifndef STATUSINDICATORDROPDOWNMENUWINDOWSTYLE_H
#define STATUSINDICATORDROPDOWNMENUWINDOWSTYLE_H

#include <mscenewindowstyle.h>

class StatusIndicatorDropDownMenuStyle : public MSceneWindowStyle
{
    Q_OBJECT
    M_STYLE(StatusIndicatorDropDownMenuStyle)

    //! controls whether to show a notification area in status indicator menu window
    M_STYLE_ATTRIBUTE(bool, notificationArea, NotificationArea)
};

class StatusIndicatorDropDownMenuStyleContainer : public MSceneWindowStyleContainer
{
    M_STYLE_CONTAINER(StatusIndicatorDropDownMenuStyle)
};

#endif
