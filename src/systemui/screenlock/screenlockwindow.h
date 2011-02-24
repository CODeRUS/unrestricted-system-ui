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
#ifndef SCREENLOCKWINDOW_H
#define SCREENLOCKWINDOW_H

#include <MWindow>
#include <X11/Xdefs.h>

class MApplicationExtensionArea;

/*!
 * Screen lock window is a window for the screen lock.
 */
class ScreenLockWindow : public MWindow
{
    Q_OBJECT

public:
    /*!
     * Creates a screen lock window.
     *
     * \param extensionArea the application extension area containing the lock screen extension.
                            The window takes ownership of the extension area.
     * \param parent the parent widget for the window
     */
    ScreenLockWindow(MApplicationExtensionArea *extensionArea, QWidget *parent = NULL);

    /*!
     * Destroys the screen lock window.
     */
    virtual ~ScreenLockWindow();

protected:
    /*!
     * Sets the _MEEGO_STACKING_LAYER window property to 2.
     */
    virtual void showEvent(QShowEvent *event);

private:
    //! Set window properties to not show the window in the switcher
    void excludeFromTaskBar();
    void changeNetWmState(bool set, Atom one, Atom two = 0);

    //! Applies the lock screen orientation and locking from the style
    void applyStyle();

#ifdef UNIT_TEST
    friend class Ut_ScreenLockWindow;
#endif
};

#endif
