#-------------------------------------------------
#
# Project created by QtCreator 2014-02-17T11:35:26
#
#-------------------------------------------------

QT       += core testlib

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
include(../../libs/PhVideo/PhVideo.pri)

HEADERS += \
	AutoTestSettings.h \
	ClockTest.h \
	SettingsTest.h \
	TimeCodeTest.h \
	DebugTest.h \
	StripDocTest.h \
	SonyControllerTest.h \
	LockableSpinBoxTest.h \
	TimeCodeEditTest.h \
	AutoTestWindow.h \
	WindowTest.h \
	GraphicTest.h \
	GraphicTextTest.h \
	GraphicStripTestSettings.h \
	GraphicStripTest.h \
	VideoTest.h \
	VideoTestSettings.h

SOURCES += main.cpp \
	ClockTest.cpp \
	SettingsTest.cpp \
	TimeCodeTest.cpp \
	DebugTest.cpp \
	StripDocTest.cpp \
	SonyControllerTest.cpp \
	LockableSpinBoxTest.cpp \
	TimeCodeEditTest.cpp \
	AutoTestWindow.cpp \
	WindowTest.cpp \
	GraphicTest.cpp \
	GraphicTextTest.cpp \
	GraphicStripTest.cpp \
	VideoTest.cpp

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/*) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/fonts/*) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/*) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/strip/*) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/video/*) . $${CS}
QMAKE_POST_LINK += rm -f *.gcda $${CS}

FORMS += \
	AutoTestWindow.ui

mac{
	# For the plist version
	QMAKE_INFO_PLIST +=  $${JOKER_ROOT}/data/test.plist
}

