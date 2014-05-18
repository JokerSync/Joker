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

QMAKE_CXXFLAGS += -g -O0 -fprofile-arcs -ftest-coverage
QMAKE_LFLAGS += -g -O0 -fprofile-arcs -ftest-coverage

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhStrip/PhStrip.pri)
include(../../libs/PhAudio/PhAudio.pri)
include(../../libs/PhSync/PhSync.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)
include(../../libs/PhGraphic/PhGraphic.pri)
include(../../libs/PhGraphicStrip/PhGraphicStrip.pri)

HEADERS += \
	AutoTestSettings.h \
	SettingsTest.h \
	TimeCodeTest.h \
	StripDocTest.h \
	SonyControllerTest.h \
	LockableSpinBoxTest.h \
	TimeCodeEditTest.h \
	AutoTestWindow.h \
	WindowTest.h \
	GraphicTest.h \
	GraphicTextTest.h \
	GraphicStripTestSettings.h \
	GraphicStripTest.h

SOURCES += main.cpp \
	SettingsTest.cpp \
	TimeCodeTest.cpp \
	StripDocTest.cpp \
	SonyControllerTest.cpp \
	LockableSpinBoxTest.cpp \
	TimeCodeEditTest.cpp \
	AutoTestWindow.cpp \
	WindowTest.cpp \
	GraphicTest.cpp \
	GraphicTextTest.cpp \
	GraphicStripTest.cpp

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

