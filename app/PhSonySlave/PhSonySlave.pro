#-------------------------------------------------
#
# Project created by QtCreator 2013-08-08T19:06:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhSonySlave
TEMPLATE = app


include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhSync/PhSync.pri)

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

