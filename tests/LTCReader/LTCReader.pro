#-------------------------------------------------
#
# Project created by QtCreator 2013-12-18T15:50:15
#
#-------------------------------------------------
cache()

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = GraphicLTC
TEMPLATE = app

DEFINES += APP_NAME=\\\"$$TARGET\\\"

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += $${JOKER_ROOT}/libs

include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)
include($${JOKER_ROOT}/libs/PhCommonUI/PhCommonUI.pri)


INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lltc -lm


SOURCES += main.cpp\
        MainWindow.cpp \
    ../../libs/PhSync/PhLtcReader.cpp

HEADERS  += MainWindow.h \
    ../../libs/PhSync/PhLtcReader.h

FORMS    += MainWindow.ui
