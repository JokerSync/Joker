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


SOURCES += main.cpp \
            ../../libs/PhStrip/PhStripDoc.cpp \

HEADERS +=    ../../libs/PhStrip/PhStripDoc.h \

INCLUDEPATH += ../../libs/ \
              /usr/include/qt5/QtXml/ \
              /usr/include/qt5/QtGui \
                ../../libs/PhStrip

DEPENDPATH += ../../libs/ \
              /usr/include/qt5/QtXml/ \
              /usr/include/qt5/QtGui \
                 ../../libs/PhStrip
