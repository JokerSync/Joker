#-------------------------------------------------
#
# Project created by QtCreator 2013-12-16T16:13:48
#
#-------------------------------------------------
cache()

QT       += core
QT       -= gui

TARGET = LTCTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lltc -lm

DEFINES += APP_NAME=\\\"$$TARGET\\\"

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..
INCLUDEPATH += $${JOKER_ROOT}/libs
include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)
