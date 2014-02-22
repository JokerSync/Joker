#-------------------------------------------------
#
# Project created by QtCreator 2014-01-27T17:15:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LTCTool
TEMPLATE = app

VERSION = 1.0.0

DEFINES += APP_NAME=\\\"$$TARGET\\\"

CONFIG += ltc

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri);
include(../../libs/PhSync/PhSync.pri);
include(../../libs/PhCommonUI/PhCommonUI.pri);

SOURCES += main.cpp\
        LTCToolWindow.cpp \
    PreferencesPanel.cpp

HEADERS  += LTCToolWindow.h \
    PreferencesPanel.h

FORMS    += LTCToolWindow.ui \
    PreferencesPanel.ui
