#-------------------------------------------------
#
# Project created by QtCreator 2013-07-29T09:40:38
#
#-------------------------------------------------

QT       += core

QT       += gui
QT       += xml

TARGET = StripTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS +=    ../../libs/PhStrip/PhStripDoc.h \
    ../../libs/PhTools/PhTimeCode.h \
    ../../libs/PhTools/PhColor.h \
    ../../libs/PhStrip/PhStripText.h \
    ../../libs/PhStrip/PhStripObject.h \
    ../../libs/PhStrip/PhStripLoop.h \
    ../../libs/PhStrip/PhStripCut.h \
    ../../libs/PhStrip/PhPeople.h

SOURCES += main.cpp \
    ../../libs/PhStrip/PhStripDoc.cpp \
    ../../libs/PhTools/PhTimeCode.cpp \
    ../../libs/PhTools/PhColor.cpp \
    ../../libs/PhStrip/PhStripText.cpp \
    ../../libs/PhStrip/PhStripObject.cpp \
    ../../libs/PhStrip/PhStripLoop.cpp \
    ../../libs/PhStrip/PhStripCut.cpp \
    ../../libs/PhStrip/PhPeople.cpp

INCLUDEPATH += ../../libs/ \
              /usr/include/qt5/QtXml/ \
              /usr/include/qt5/QtGui \
                ../../libs/PhStrip

DEPENDPATH += ../../libs/ \
              /usr/include/qt5/QtXml/ \
              /usr/include/qt5/QtGui \
                 ../../libs/PhStrip

