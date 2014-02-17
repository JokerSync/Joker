#-------------------------------------------------
#
# Project created by QtCreator 2014-02-17T11:35:26
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = AutoTest
CONFIG   += console static
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhStrip/PhStrip.pri)
include(../../libs/PhSync/PhSync.pri)

SOURCES += main.cpp
