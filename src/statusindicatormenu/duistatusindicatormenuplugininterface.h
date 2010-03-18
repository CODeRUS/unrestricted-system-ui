/*
 * This file is part of system ui
 *
 * Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 * All rights reserved.
 *
 * Contact: Tomas Junnonen <tomas.junnonen@nokia.com>
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved. Copying,
 * including reproducing, storing, adapting or translating, any or all of
 * this material requires the prior written consent of Nokia Corporation.
 * This material also contains confidential information which may not be
 * disclosed to others without the prior written consent of Nokia.
 */

#ifndef DUISTATUSINDICATORMENUPLUGININTERFACE_H
#define DUISTATUSINDICATORMENUPLUGININTERFACE_H

#include <QtPlugin>

class DuiWidget;

/**
 * The DuiStatusIndicatorMenuInterface interface represents a status indicator
 * menu application, its window and main page. The interface can be used to
 * show the main status indicator menu application page or to hide the status
 * indicator menu window.
 */
class DuiStatusIndicatorMenuInterface
{
public:
    /*!
     * Shows the main status indicator menu application page.
     */
    virtual void showStatusIndicatorMenu() = 0;

    /*!
     * Hides the status indicator menu window.
     */
    virtual void hideStatusIndicatorMenu() = 0;
};

/**
 * DuiStatusIndicatorMenuPluginInterface is the base class for status indicator
 * menu plugin classes.
 *
 * Plugin developers need to implement this interface in their plugin binary
 * and export their implementation from the binary. The host application will maintain
 * an inventory of available plugins and instantiate them using this interface
 * when needed.
 */
class DuiStatusIndicatorMenuPluginInterface
{
public:
    /*!
     * Destroys the plugin.
     */
    virtual ~DuiStatusIndicatorMenuPluginInterface() {}

    /*!
     * This method is called to construct a new DuiWidget instance.
     * Caller will maintain the ownership of the constructed widget and
     * will destroy it when it is no longer needed.
     *
     * \param statusIndicatorMenu an interface for controlling the status indicator menu
     * \return New DuiWidget class instance that visualizes this plugin.
     */
    virtual DuiWidget *constructWidget(DuiStatusIndicatorMenuInterface &statusIndicatorMenu) = 0;
};

Q_DECLARE_INTERFACE(DuiStatusIndicatorMenuPluginInterface, "com.nokia.dui.core.StatusIndicatorMenuPluginInterface/1.0")

#endif // DUISTATUSINDICATORMENUPLUGININTERFACE_H
