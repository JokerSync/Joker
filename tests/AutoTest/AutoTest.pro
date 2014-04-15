#-------------------------------------------------
#
# Project created by QtCreator 2014-02-17T11:35:26
#
#-------------------------------------------------

QT       += core

TARGET = AutoTest
CONFIG   += console static
CONFIG   -= app_bundle

TEMPLATE = app

QMAKE_CXXFLAGS += -g -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -fprofile-arcs -ftest-coverage  -O0

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhStrip/PhStrip.pri)
include(../../libs/PhAudio/PhAudio.pri)
include(../../libs/PhSync/PhSync.pri)
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
    AutoTestWindow.h

SOURCES += main.cpp \
    StripDocTest.cpp \
    LockableSpinBoxTest.cpp \
    SettingsTest.cpp \
    TimeCodeEditTest.cpp \
    TimeCodeTest.cpp \
    SonyControllerTest.cpp \
    WindowTest.cpp \
    AutoTestWindow.cpp


QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/strip/*) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/text.txt) . $${CS}

FORMS += \
    AutoTestWindow.ui
