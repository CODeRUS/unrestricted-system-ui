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
#include "screenlockbusinesslogic.h"
#include "screenlockwindow.h"
#include "eventeater.h"
#include "closeeventeater.h"

ScreenLockBusinessLogic::ScreenLockBusinessLogic(QObject* parent) :
    QObject(parent),
    screenLockWindow(new ScreenLockWindow),
    eventEaterWindow(new EventEater)
{
    // Install a close event eater for the windows
    CloseEventEater *closeEventEater = new CloseEventEater(this);
    screenLockWindow->installEventFilter(closeEventEater);
    eventEaterWindow->installEventFilter(closeEventEater);

    connect(screenLockWindow, SIGNAL(unlocked()), this, SLOT(unlockScreen()));
    connect(screenLockWindow, SIGNAL(unlocked()), this, SIGNAL(unlockConfirmed()));

    // Hide the event eater when it is clicked
    connect(eventEaterWindow, SIGNAL(inputEventReceived()), this, SLOT(hideEventEater()));

#ifdef HAVE_QMSYSTEM
    connect(&displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), this, SLOT(displayStateChanged(MeeGo::QmDisplayState::DisplayState)));
    connect(&locks, SIGNAL(stateChanged(MeeGo::QmLocks::Lock, MeeGo::QmLocks::State)), this, SLOT(locksChanged(MeeGo::QmLocks::Lock, MeeGo::QmLocks::State)));
#endif

}

ScreenLockBusinessLogic::~ScreenLockBusinessLogic()
{
    delete screenLockWindow;
    delete eventEaterWindow;
}

void ScreenLockBusinessLogic::unlockScreen()
{
    toggleScreenLockUI(false);
}

#ifdef HAVE_QMSYSTEM
void ScreenLockBusinessLogic::displayStateChanged(MeeGo::QmDisplayState::DisplayState state)
{
    // When the screen is unlocked, the screenlock is visible the lock screen needs to be reset
    if (state == MeeGo::QmDisplayState::On && screenLockWindow->isVisible()) {
        screenLockWindow->reset();
        screenLockWindow->setFocus();
    }
}
#endif

void ScreenLockBusinessLogic::hideEventEater()
{
    toggleEventEater(false);
}

void ScreenLockBusinessLogic::toggleScreenLockUI(bool toggle)
{
    if (toggle) {
        // Whenever we're showing the lock screen we need to reset its state
        screenLockWindow->reset();

        if (!screenLockWindow->isVisible()) {
            screenLockWindow->show();
        }

        screenLockWindow->raise();
    } else {
        if (screenLockWindow->isVisible()) {
            screenLockWindow->hide();
        }
    }
}

void ScreenLockBusinessLogic::toggleEventEater(bool toggle)
{
    if (toggle) {
        if (!eventEaterWindow->isVisible()) {
            eventEaterWindow->show();
            eventEaterWindow->showFullScreen();
        }
    } else {
        if (eventEaterWindow->isVisible()) {
            eventEaterWindow->hide();
        }
    }
}

bool ScreenLockBusinessLogic::displayIsOn()
{
#if !defined(__i386__) && defined(HAVE_QMSYSTEM)
    return displayState.get() == MeeGo::QmDisplayState::On;
#else
    return true;
#endif
}

#ifdef HAVE_QMSYSTEM
void ScreenLockBusinessLogic::locksChanged(MeeGo::QmLocks::Lock what, MeeGo::QmLocks::State how)
{
    if (what == MeeGo::QmLocks::TouchAndKeyboard) {
        switch (how) {
        case MeeGo::QmLocks::Locked:
            emit screenIsLocked(true);
            break;
        default:
            emit screenIsLocked(false);
            break;
        }
    }
}
#endif