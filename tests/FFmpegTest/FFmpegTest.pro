#-------------------------------------------------
#
# Project created by QtCreator 2013-10-01T16:48:45
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/QtAV

TARGET = FFmpegTest
TEMPLATE = app

SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

LIBS += -L/usr/local/lib
LIBS += -lQtAV

QMAKE_POST_LINK += cp /usr/local/lib/libQtA*.dylib $${TARGET}.app/Contents/MacOS;
