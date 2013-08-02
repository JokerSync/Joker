#-------------------------------------------------
#
# Project created by QtCreator 2013-08-01T12:25:30
#
#-------------------------------------------------

QT       += core gui
QT		+= opengl
QT       += core gui
QT		+= xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicTest
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
	GraphicTestView.cpp \
	../../libs/PhGraphic/PhGraphicView.cpp \
	../../libs/PhGraphic/PhGraphicContext.cpp \
	../../libs/PhGraphic/PhGraphicImage.cpp \
	../../libs/PhGraphic/PhGraphicText.cpp \
    ../../libs/PhGraphic/PhGraphicTexturedRect.cpp \
    ../../libs/PhTools/PhFont.cpp \
    ../../libs/PhTools/PhColor.cpp \
    ../../libs/PhGraphic/PhGraphicObject.cpp \
    ../../libs/PhGraphic/PhGraphicRect.cpp \
    ../../libs/PhTools/PhString.cpp

HEADERS  += MainWindow.h \
	GraphicTestView.h \
	../../libs/PhGraphic/PhGraphicView.h \
	../../libs/PhGraphic/PhGraphicContext.h \
	../../libs/PhGraphic/PhGraphicImage.h \
	../../libs/PhGraphic/PhGraphicText.h \
    ../../libs/PhGraphic/PhGraphicTexturedRect.h \
    ../../libs/PhTools/PhFont.h \
    ../../libs/PhTools/PhColor.h \
    ../../libs/PhGraphic/PhGraphicObject.h \
    ../../libs/PhGraphic/PhGraphicRect.h \
    ../../libs/PhTools/PhString.h

INCLUDEPATH += ../../libs/PhGraphic \
				../../libs/PhTools \
				 /usr/include/GL \
			   /usr/include/SDL \
				../../libs

DEPENDPATH +=  ../../libs/PhGraphic \
				../../libs/PhTools \
				 /usr/include/GL \
			   /usr/include/SDL \
				../../libs

LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
