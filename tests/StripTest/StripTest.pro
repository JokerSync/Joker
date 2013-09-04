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

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhStrip/PhStrip.pri)

SOURCES += main.cpp

INCLUDEPATH += ../../libs


