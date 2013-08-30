#-------------------------------------------------
#
# Project created by QtCreator 2013-08-29T12:28:34
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialTest
TEMPLATE = app

include(../../libs/PhTools/PhTools.pri)

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui
