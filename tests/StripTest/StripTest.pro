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
	../../libs/PhTools/PhColor.cpp \
	../../libs/PhTools/PhTime.cpp \
    ../../libs/PhTools/PhTimeCode.cpp \
    ../../libs/PhStrip/PhStripObject.cpp \
    ../../libs/PhStrip/PhStripCut.cpp \
    ../../libs/PhStrip/PhStripText.cpp \
    ../../libs/PhStrip/PhStripLoop.cpp \
    ../../libs/PhStrip/PhPeople.cpp \
	../../libs/PhTools/PhString.cpp

HEADERS += \
    phstripdoc.h \
	../../libs/PhTools/PhColor.h \
	../../libs/PhTools/PhTime.h \
    ../../libs/PhTools/PhTimeCode.h \
    ../../libs/PhStrip/PhStripObject.h \
    ../../libs/PhStrip/PhStripCut.h \
    ../../libs/PhStrip/PhStripText.h \
    ../../libs/PhStrip/PhStripLoop.h \
    ../../libs/PhStrip/PhPeople.h \
	../../libs/PhTools/PhString.h

INCLUDEPATH += 	../../libs \
