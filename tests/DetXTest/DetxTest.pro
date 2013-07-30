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
        ../../libs/PhStrip/PhStripDoc.cpp \
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
          ../../libs/PhStrip/PhStripDoc.h \
	../../libs/PhTools/PhColor.h \
	../../libs/PhTools/PhTime.h \
	../../libs/PhTools/PhTimeCode.h \
	../../libs/PhStrip/PhStripObject.h \
	../../libs/PhStrip/PhStripCut.h \
	../../libs/PhStrip/PhStripText.h \
	../../libs/PhStrip/PhStripLoop.h \
	../../libs/PhStrip/PhPeople.h \
	../../libs/PhTools/PhString.h

INCLUDEPATH += \
	../StripTest/ \
	../../libs/

