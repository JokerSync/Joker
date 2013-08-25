#-------------------------------------------------
#
# Project created by QtCreator 2013-05-24T11:52:48
#
#-------------------------------------------------

QT      += core

QT       -= gui

TARGET = DetXTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += $${JOKER_ROOT}/libs

include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)
include($${JOKER_ROOT}/libs/PhStrip/PhStrip.pri)

SOURCES += main.cpp

