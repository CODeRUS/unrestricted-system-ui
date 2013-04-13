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
#ifndef NETSPEEDINDICATOR_STUB
#define NETSPEEDINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>

#ifdef HAVE_QMSYSTEM
#include <qmdevicemode.h>
#endif

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class NetSpeedIndicatorStub : public StubBase
{
public:
    virtual void NetSpeedIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent);
    virtual void NetSpeedIndicatorDestructor();
    virtual void gconf_enable_changed();
    virtual void gconf_when_online_changed();
    virtual void updateStatusIndicator();
    virtual void onlineChanged(bool);
};

// 2. IMPLEMENT STUB
void NetSpeedIndicatorStub::NetSpeedIndicatorConstructor(ApplicationContext &context, QGraphicsItem *parent)
{
    Q_UNUSED(context);
    Q_UNUSED(parent);

}
void NetSpeedIndicatorStub::NetSpeedIndicatorDestructor()
{

}

void NetSpeedIndicatorStub::gconf_enable_changed()
{
    stubMethodEntered("gconf_enable_changed");
}

void NetSpeedIndicatorStub::gconf_when_online_changed()
{
    stubMethodEntered("gconf_when_online_changed");
}

void NetSpeedIndicatorStub::updateStatusIndicator()
{
  stubMethodEntered("updateStatusIndicator");
}

void NetSpeedIndicatorStub::onlineChanged(bool)
{
  stubMethodEntered("onlineChanged");
}

// 3. CREATE A STUB INSTANCE
NetSpeedIndicatorStub gDefaultNetSpeedIndicatorStub;
NetSpeedIndicatorStub *gNetSpeedIndicatorStub = &gDefaultNetSpeedIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
NetSpeedIndicator::NetSpeedIndicator(ApplicationContext &context, QGraphicsItem *parent)
{
    gNetSpeedIndicatorStub->NetSpeedIndicatorConstructor(context, parent);
}

NetSpeedIndicator::~NetSpeedIndicator()
{
    gNetSpeedIndicatorStub->NetSpeedIndicatorDestructor();
}

void NetSpeedIndicator::gconf_enable_changed()
{
    gNetSpeedIndicatorStub->gconf_enable_changed();
}

void NetSpeedIndicator::gconf_when_online_changed()
{
    gNetSpeedIndicatorStub->gconf_when_online_changed();
}

void NetSpeedIndicator::updateStatusIndicator()
{
  gNetSpeedIndicatorStub->updateStatusIndicator();
}

void NetSpeedIndicator::onlineChanged(bool e)
{
  gNetSpeedIndicatorStub->onlineChanged(e);
}
#endif // NETSPEEDINDICATOR_STUB
