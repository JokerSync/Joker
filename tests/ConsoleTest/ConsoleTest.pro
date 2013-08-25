#-------------------------------------------------
#
# Project created by QtCreator 2013-05-27T17:36:08
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ConsoleTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)

SOURCES += main.cpp

INCLUDEPATH += 	$${JOKER_ROOT}/libs

RESOURCES_PATH = .

QMAKE_POST_LINK += cp $${JOKER_ROOT}/data/img/look.png $${RESOURCES_PATH}
#QMAKE_POST_LINK += echo $${_PRO_FILE_PWD_} && echo test
