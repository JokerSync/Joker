#-------------------------------------------------
#
# Project created by QtCreator 2014-02-17T11:35:26
#
#-------------------------------------------------

QT       += core

TARGET = AutoTest
CONFIG   += console static debug
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -g -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -fprofile-arcs -ftest-coverage  -O0

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhStrip/PhStrip.pri)
include(../../libs/PhAudio/PhAudio.pri)
include(../../libs/PhSync/PhSync.pri)
include(../../libs/PhGraphic/PhGraphic.pri)
include(../../libs/PhGraphicStrip/PhGraphicStrip.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)

HEADERS += \
    StripDocTest.h \
    LockableSpinBoxTest.h \
    SettingsTest.h \
    TimeCodeEditTest.h \
    TimeCodeTest.h \
    SonyControllerTest.h \
    AutoTestSettings.h \
    WindowTest.h \
	AutoTestWindow.h \
	GraphicStripTest.h \
    GraphicTextTest.h \
    GraphicStripTestSettings.h

SOURCES += main.cpp \
    StripDocTest.cpp \
    LockableSpinBoxTest.cpp \
    SettingsTest.cpp \
    TimeCodeEditTest.cpp \
    TimeCodeTest.cpp \
    SonyControllerTest.cpp \
    WindowTest.cpp \
	AutoTestWindow.cpp \
	GraphicStripTest.cpp \
    GraphicTextTest.cpp

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/*) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/fonts/*) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/*) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/strip/*) . $${CS}

DEFINES += PATH_TO_RESSOURCES=\\\"\\\"

FORMS += \
    AutoTestWindow.ui

mac{
	# For the plist version
	QMAKE_INFO_PLIST +=  $${JOKER_ROOT}/data/test.plist
}

