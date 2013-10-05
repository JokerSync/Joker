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

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)


SOURCES += main.cpp\
        MainWindow.cpp \
    AboutMenu.cpp

HEADERS  += MainWindow.h \
    AboutMenu.h

FORMS    += MainWindow.ui \
    AboutMenu.ui
