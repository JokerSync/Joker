#-------------------------------------------------
#
# Project created by QtCreator 2013-10-03T18:26:17
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGLTest
TEMPLATE = app


SOURCES += main.cpp\
        OpenGLTestWindow.cpp

HEADERS  += OpenGLTestWindow.h

QMAKE_CXXFLAGS += -F/Library/Frameworks
LIBS += -F/Library/Frameworks

LIBS += -framework SDL2 -framework SDL2_image
