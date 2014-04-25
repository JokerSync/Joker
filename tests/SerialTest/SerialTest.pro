#-------------------------------------------------
#
# Project created by QtCreator 2013-08-29T12:28:34
#
#-------------------------------------------------

QT       += core gui serialport widgets

TARGET = SerialTest
TEMPLATE = app

VERSION = 1.0.0

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)

SOURCES += main.cpp\
        SerialTestWindow.cpp

HEADERS  += SerialTestWindow.h \
    SerialTestSettings.h

FORMS    += SerialTestWindow.ui
