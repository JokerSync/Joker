#-------------------------------------------------
#
# Project created by QtCreator 2013-08-14T20:05:01
#
#-------------------------------------------------

QT       += core gui
QT += widgets

#greaterThan(QT_MAJOR_VERSION, 4): 

TARGET = SonyTest
TEMPLATE = app

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs//PhCommonUI/PhCommonUI.pri)
include(../../libs/PhSync/PhSync.pri)

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui
