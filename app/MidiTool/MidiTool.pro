#-------------------------------------------------
#
# Project created by QtCreator 2014-01-27T17:15:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MidiTool
TEMPLATE = app

VERSION = 1.0.0

DEFINES += APP_NAME=\\\"$$TARGET\\\"

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri);
include(../../libs/PhMidi/PhMidi.pri);
include(../../libs/PhCommonUI/PhCommonUI.pri);

SOURCES += main.cpp\
        MidiToolWindow.cpp \
    PreferencesDialog.cpp

HEADERS  += MidiToolWindow.h \
    PreferencesDialog.h \
    MidiToolSettings.h

FORMS    += MidiToolWindow.ui \
    PreferencesDialog.ui

mac {
	# For the plist version
	QMAKE_INFO_PLIST +=  $${JOKER_ROOT}/data/test.plist
}

PH_DEPLOY_LOCATION = $$(JOKER_RELEASE_PATH)
include(../../common/deploy.pri)
