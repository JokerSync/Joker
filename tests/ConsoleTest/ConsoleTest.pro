#-------------------------------------------------
#
# Project created by QtCreator 2013-05-27T17:36:08
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ConsoleTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH += 	../../libs \

# PhTools

HEADERS += \
	../../libs/PhTools/PhTime.h \
    ../../libs/PhTools/PhTimeCode.h

SOURCES += \
	../../libs/PhTools/PhTime.cpp \
	../../libs/PhTools/PhTimeCode.cpp

