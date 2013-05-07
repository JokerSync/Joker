QT       += core gui
QT		 += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGLTest
TEMPLATE = app


SOURCES += \
	main.cpp\
    myglwidget.cpp \
    mywindow.cpp \
    SDLMain.cpp

HEADERS  += \
    myglwidget.h \
	mywindow.h

FORMS    += mainwindow.ui

LIBS += -framework SDL -framework SDL_image -framework SDL_ttf

INCLUDEPATH += /Library/Frameworks/
DEPENDPATH += /Library/Frameworks/



