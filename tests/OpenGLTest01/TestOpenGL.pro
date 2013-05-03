#-------------------------------------------------
#
# Project created by QtCreator 2013-04-25T11:26:46
#
#-------------------------------------------------

QT       += core gui
QT		 += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestOpenGL
TEMPLATE = app


SOURCES += main.cpp\
    myglwidget.cpp \
    mywindow.cpp

HEADERS  += \
    myglwidget.h \
	mywindow.h \
    SDLMain.h

FORMS    += mainwindow.ui
OBJECTIVE_SOURCES += SDLMain.m

INCLUDEPATH += /Library/Frameworks/SDL.framework/Headers
LIBS += -framework Cocoa -framework SDL

INCLUDEPATH += /Library/Frameworks
DEPENDPATH += /Library/Frameworks



