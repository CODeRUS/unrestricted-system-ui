MOC_DIR = .moc
MGEN_OUTDIR = .gen
OBJECTS_DIR = .obj

include(../../../mconfig.pri)

TEMPLATE      = lib
CONFIG       += plugin gui meegotouch duistatusindicatormenu silent
INCLUDEPATH  += \
    ../../../src/statusindicatormenu

MODEL_HEADERS =
STYLE_HEADERS =

HEADERS = \
    profileplugin.h \
    profile.h \
    $$MODEL_HEADERS \
    $$STYLE_HEADERS

SOURCES = \
    profileplugin.cpp \
    profile.cpp \

TARGET        = $$qtLibraryTarget(profile)
DESTDIR       = ../../../lib

target.path += $$DUISTATUSINDICATORMENU_PLUGIN_DIR

INSTALLS += \
    target \
    svg \
    images

svg.path = $$DUISTATUSINDICATORMENU_THEMES_DIR/svg
svg.files = images/*.svg
images.path = $$DUISTATUSINDICATORMENU_THEMES_DIR/images
images.files = images/*.png

include(../check.pri)