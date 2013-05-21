#-------------------------------------------------
#
# Project created by QtCreator 2013-05-21T10:21:25
#
#-------------------------------------------------

QT       += core gui
QT		 += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = videoTest
TEMPLATE = app


SOURCES += main.cpp\
        videotest.cpp \
	../../libs/PhGraphic/phglwidget.cpp

HEADERS  += videotest.h \
	../../libs/PhGraphic/phglwidget.h

FORMS    += videotest.ui

INCLUDEPATH += /Library/Frameworks/ \
			../../libs

DEPENDPATH += /Library/Frameworks
