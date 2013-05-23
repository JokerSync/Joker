#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T14:09:50
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = StripTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    phstripdoc.cpp \
    ../../libs/PhStrip/phstring.cpp \
    ../../libs/PhGraphic/phcolor.cpp \
    ../../libs/PhSync/phtime.cpp

HEADERS += \
    phstripdoc.h \
    ../../libs/PhStrip/phstring.h \
    ../../libs/PhGraphic/phcolor.h \
    ../../libs/PhSync/phtime.h

INCLUDEPATH += 	../../libs \
