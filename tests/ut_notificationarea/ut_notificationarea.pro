include(../common_top.pri)
TARGET = ut_notificationarea
INCLUDEPATH += $$SRCDIR/notifications $$SRCDIR/statusindicatormenu

STYLE_HEADERS += $$SRCDIR/statusindicatormenu/notificationareastyle.h
MODEL_HEADERS += $$SRCDIR/statusindicatormenu/notificationareamodel.h

# unit test and unit
SOURCES += \
    ut_notificationarea.cpp \
    $$SRCDIR/statusindicatormenu/notificationarea.cpp

# base classes
SOURCES += \

# service classes
SOURCES += \
    $$SRCDIR/statusindicatormenu/notificationareaview.cpp \
    $$STUBSDIR/stubbase.cpp

# unit test and unit
HEADERS += \
    ut_notificationarea.h \
    $$SRCDIR/statusindicatormenu/notificationarea.h \
    $$SRCDIR/statusindicatormenu/notificationareamodel.h

# base classes
HEADERS += \


# service classes
HEADERS += \
    $$SRCDIR/notifications/notificationsink.h \
    $$SRCDIR/notifications/widgetnotificationsink.h \
    $$SRCDIR/notifications/notificationareasink.h \
    $$SRCDIR/statusindicatormenu/notificationareaview.h \
    $$SRCDIR/statusindicatormenu/notificationareastyle.h \
    $$SRCDIR/sysuid.h

include(../common_bot.pri)
