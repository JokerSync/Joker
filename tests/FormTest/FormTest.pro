#-------------------------------------------------
#
# Project created by QtCreator 2013-09-16T10:46:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FormTest
TEMPLATE = app

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += $${JOKER_ROOT}/libs

include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)
include($${JOKER_ROOT}/libs/PhCommonUI/PhCommonUI.pri)


SOURCES += main.cpp\
        MainWindow.cpp \
    Dialog.cpp

HEADERS  += MainWindow.h \
    Dialog.h

FORMS    += MainWindow.ui \
    Dialog.ui

OTHER_FILES +=
