QT       += core gui
QT		 += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TextTest
TEMPLATE = app


SOURCES += \
	main.cpp\
    SDLMain.cpp \
	../../libs/PhTools/memorytool.cpp \
    ../../libs/PhGraphic/PhGraphicView.cpp \
    TextTestWindow.cpp

HEADERS  += \
	../../libs/PhTools/memorytool.h \
    ../../libs/PhGraphic/PhGraphicView.h \
    TextTestWindow.h

RESOURCES += \
	../../data.qrc

FORMS    += mainwindow.ui

LIBS += -framework SDL -framework SDL_image -framework SDL_ttf

INCLUDEPATH += /Library/Frameworks/ \
			../../libs \

DEPENDPATH += /Library/Frameworks

