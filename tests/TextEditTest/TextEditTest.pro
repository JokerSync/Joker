#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T23:26:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TextEditTest
TEMPLATE = app

# The application version
VERSION = 1.0.0

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)

SOURCES += main.cpp\
        TextEditTestWindow.cpp

HEADERS  += TextEditTestWindow.h \
    TextEditTestSettings.h

FORMS    += TextEditTestWindow.ui
