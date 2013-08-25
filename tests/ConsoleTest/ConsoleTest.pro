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

SOURCES += main.cpp

INCLUDEPATH += 	$${JOKER_ROOT}/libs

# PhTools

HEADERS += \
	../../libs/PhTools/PhTime.h \
    ../../libs/PhTools/PhTimeCode.h

SOURCES += \
	../../libs/PhTools/PhTime.cpp \
	../../libs/PhTools/PhTimeCode.cpp

RESOURCES_PATH = .

QMAKE_POST_LINK += cp $${JOKER_ROOT}/data/img/look.png $${RESOURCES_PATH}
#QMAKE_POST_LINK += echo $${_PRO_FILE_PWD_} && echo test
