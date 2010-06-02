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
#ifndef _UT_STATUSAREARENDERER_
#define _UT_STATUSAREARENDERER_

#include <QtTest/QtTest>
#include "qmdisplaystate.h"

class StatusAreaRenderer;
class MApplication;

class Ut_StatusAreaRenderer : public QObject
{
    Q_OBJECT

private:
    StatusAreaRenderer *statusAreaWindow;
    MApplication *app;

signals:
    void changed(QList<QRectF> rectList);
    void displayStateChanged(Maemo::QmDisplayState::DisplayState);

private slots:
    // Executed once before every test case
    void init();
    // Executed once after every test case
    void cleanup();
    // Executed once before first test case
    void initTestCase();
    // Executed once after last test case
    void cleanupTestCase();
    // Test scene changed events render the scene
    void testSceneChanged();
    // Test rendering happens when display state is on
    void testSceneRenderControlDisplayStateOn();
    // Test rendering does not happen when display state is off
    void testSceneRenderControlDisplayStateOff();
    // Test rendering does not happen when display state is dimmed
    void testSceneRenderControlDisplayStateDimmed();
    // Test MOnDisplayChangeEvent dispatching
    void testMOnDisplayChangeEvent();
};

class RenderTestsHelper
{
public:
    RenderTestsHelper();
    ~RenderTestsHelper();
    QList<QRectF>* setupRenderTests(Ut_StatusAreaRenderer* testClass, StatusAreaRenderer* statusAreaWindow);
private:
    QList<QRectF>* rectList;
    void setupRect();
};

#endif //_UT_STATUSAREARENDERER_
