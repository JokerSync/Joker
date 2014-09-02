#-------------------------------------------------
#
# Project created by QtCreator 2014-02-17T11:35:26
#
#-------------------------------------------------

QT       += testlib

TARGET = AutoTest
CONFIG   += console static debug
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -g -O0 -fprofile-arcs -ftest-coverage
QMAKE_LFLAGS += -g -O0 -fprofile-arcs -ftest-coverage

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($$TOP_ROOT/libs/PhTools/PhTools.pri)
include($$TOP_ROOT/libs/PhStrip/PhStrip.pri)
include($$TOP_ROOT/libs/PhAudio/PhAudio.pri)
include($$TOP_ROOT/libs/PhSync/PhSync.pri)
include($$TOP_ROOT/libs/PhSony/PhSony.pri)
include($$TOP_ROOT/libs/PhLtc/PhLtc.pri)
include($$TOP_ROOT/libs/PhCommonUI/PhCommonUI.pri)
include($$TOP_ROOT/libs/PhGraphic/PhGraphic.pri)
include($$TOP_ROOT/libs/PhGraphicStrip/PhGraphicStrip.pri)
include($$TOP_ROOT/libs/PhVideo/PhVideo.pri)
include($$TOP_ROOT/libs/PhMidi/PhMidi.pri)

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
	VideoTestSettings.h \
	MidiTest.h \
    SynchronizerTest.h

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
	VideoTest.cpp \
	MidiTest.cpp \
    SynchronizerTest.cpp

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/*) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/fonts/*) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/*) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} -r $$shell_path($${TOP_ROOT}/data/strip/) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/video/*) . $${CS}
QMAKE_POST_LINK += rm -f *.gcda $${CS}

FORMS += \
	AutoTestWindow.ui

mac{
	# For the plist version
	QMAKE_INFO_PLIST +=  $${TOP_ROOT}/data/test.plist
}

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
