#-------------------------------------------------
#
# Project created by QtCreator 2013-11-13T10:56:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicSyncTest
TEMPLATE = app

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhSync/PhSync.pri)
include(../../libs/PhGraphic/PhGraphic.pri)

SOURCES += main.cpp\
        GraphicSyncTestWindow.cpp \
    GraphicSyncTestView.cpp

HEADERS  += GraphicSyncTestWindow.h \
    GraphicSyncTestView.h

FORMS    += GraphicSyncTestWindow.ui
