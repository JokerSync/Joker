#-------------------------------------------------
#
# Project created by QtCreator 2013-05-24T11:52:48
#
#-------------------------------------------------

QT      += core
QT		+= xml

QT       -= gui

TARGET = DetxTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ../StripTest/phstripdoc.cpp

HEADERS += \
    ../StripTest/phstripdoc.h

INCLUDEPATH += \
	../StripTest/ \
	../../libs/

