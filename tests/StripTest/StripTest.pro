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

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += $${JOKER_ROOT}/libs

include($${JOKER_ROOT}/libs/PhStrip/PhStrip.pri)
include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)

#Main app
SOURCES += main.cpp \
