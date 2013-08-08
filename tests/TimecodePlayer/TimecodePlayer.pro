#-------------------------------------------------
#
# Project created by QtCreator 2013-08-06T16:37:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimecodePlayer
TEMPLATE = app

INCLUDEPATH = ../../libs

SOURCES += main.cpp\
        mainwindow.cpp \
    ../../libs/PhTools/PhTimeCode.cpp \
    ../../libs/PhTools/PhTime.cpp \
    ../../libs/PhTools/PhString.cpp

HEADERS  += mainwindow.h \
    ../../libs/PhTools/PhTimeCode.h \
    ../../libs/PhTools/PhTime.h \
    ../../libs/PhTools/PhString.h

FORMS    += mainwindow.ui
