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
    ../../libs/PhTools/PhString.cpp \
    ../../libs/PhTools/PhClock.cpp \
    ../../libs/PhCommonUI/PhMediaControllerView.cpp

HEADERS  += mainwindow.h \
    ../../libs/PhTools/PhTimeCode.h \
    ../../libs/PhTools/PhString.h \
    ../../libs/PhTools/PhClock.h \
    ../../libs/PhCommonUI/PhMediaControllerView.h

FORMS += \
    ../../libs/PhCommonUI/PhMediaControllerView.ui
