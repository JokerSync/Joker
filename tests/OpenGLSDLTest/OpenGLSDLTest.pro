QT       += core gui
QT		 += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenGLTest
TEMPLATE = app


SOURCES += \
	main.cpp\
    SDLMain.cpp \
    TestSDLOpenGLWidget.cpp \
    ../../libs/PhGraphic/phglwidget.cpp

HEADERS  += \
    TestSDLOpenGLWidget.h \
    ../../libs/PhGraphic/phglwidget.h

FORMS    += mainwindow.ui

LIBS += -framework SDL -framework SDL_image -framework SDL_ttf

INCLUDEPATH += /Library/Frameworks/ \
			../../libs/PhGraphic/
DEPENDPATH += /Library/Frameworks/ \
			../../libs/PhGraphic/



