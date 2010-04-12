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
#ifndef SYSUID_STUB
#define SYSUID_STUB

#include "sysuid.h"
#include <stubbase.h>
#include <MApplicationWindow>

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class SysuidStub : public StubBase {
  public:
  virtual void SysuidConstructor();
  virtual void SysuidDestructor();
  virtual Sysuid * sysuid();
  virtual QString dbusService();
  virtual QString dbusPath();
  virtual NotificationManager & notificationManager();
  virtual MCompositorNotificationSink & compositorNotificationSink();
  virtual M::Orientation orientation() const;
  virtual M::OrientationAngle orientationAngle() const;
  virtual MApplicationWindow &applicationWindow();
  virtual void retranslate();
  virtual void applyUseMode();
}; 

// 2. IMPLEMENT STUB
void SysuidStub::SysuidConstructor() {

}
void SysuidStub::SysuidDestructor() {

}
Sysuid * SysuidStub::sysuid() {
  stubMethodEntered("sysuid");
  return stubReturnValue<Sysuid *>("sysuid");
}

QString SysuidStub::dbusService() {
  stubMethodEntered("dbusService");
  return stubReturnValue<QString>("dbusService");
}

QString SysuidStub::dbusPath() {
  stubMethodEntered("dbusPath");
  return stubReturnValue<QString>("dbusPath");
}

NotificationManager & SysuidStub::notificationManager() {
  stubMethodEntered("notificationManager");
  return stubReturnValue<NotificationManager &>("notificationManager");
}

MCompositorNotificationSink & SysuidStub::compositorNotificationSink() {
  stubMethodEntered("compositorNotificationSink");
  return stubReturnValue<MCompositorNotificationSink &>("compositorNotificationSink");
}

M::Orientation SysuidStub::orientation() const {
  stubMethodEntered("orientation");
  return stubReturnValue<M::Orientation>("orientation");
}

M::OrientationAngle SysuidStub::orientationAngle() const {
  stubMethodEntered("orientationAngle");
  return stubReturnValue<M::OrientationAngle>("orientationAngle");
}

MApplicationWindow &SysuidStub::applicationWindow() {
  static MApplicationWindow window;
  stubMethodEntered("applicationWindow");
  return window;
}

void SysuidStub::retranslate() {
  stubMethodEntered("retranslate");
}

void SysuidStub::applyUseMode() {
    stubMethodEntered("applyUseMode");
}


// 3. CREATE A STUB INSTANCE
SysuidStub gDefaultSysuidStub;
SysuidStub* gSysuidStub = &gDefaultSysuidStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
Sysuid::Sysuid() {
  gSysuidStub->SysuidConstructor();
}

Sysuid::~Sysuid() {
  gSysuidStub->SysuidDestructor();
}

Sysuid * Sysuid::sysuid() {
  return gSysuidStub->sysuid();
}

QString Sysuid::dbusService() {
  return gSysuidStub->dbusService();
}

QString Sysuid::dbusPath() {
  return gSysuidStub->dbusPath();
}

NotificationManager & Sysuid::notificationManager() {
  return gSysuidStub->notificationManager();
}

MCompositorNotificationSink & Sysuid::compositorNotificationSink() {
  return gSysuidStub->compositorNotificationSink();
}

M::Orientation Sysuid::orientation() const {
  return gSysuidStub->orientation();
}

M::OrientationAngle Sysuid::orientationAngle() const {
  return gSysuidStub->orientationAngle();
}

MApplicationWindow &Sysuid::applicationWindow() {
  return gSysuidStub->applicationWindow();
}

void Sysuid::retranslate() {
  gSysuidStub->retranslate();
}

void Sysuid::applyUseMode() {
  gSysuidStub->applyUseMode();
}

#endif