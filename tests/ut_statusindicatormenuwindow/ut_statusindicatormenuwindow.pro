include(../common_top.pri)
TARGET = ut_statusindicatormenuwindow
INCLUDEPATH += $$SRCDIR/statusindicatormenu

# unit test and unit classes
SOURCES += \
    ut_statusindicatormenuwindow.cpp \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.cpp

# service classes
SOURCES += \
    $$STUBSDIR/stubbase.cpp

# unit test and unit classes
HEADERS += \
    ut_statusindicatormenuwindow.h \
    $$SRCDIR/statusindicatormenu/statusindicatormenuwindow.h \
    $$SRCDIR/statusindicatormenu/pluginlist.h \

include(../common_bot.pri)