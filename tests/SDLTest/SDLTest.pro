QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TEMPLATE = app


SOURCES += main.cpp \
    ../../libs/PhTools/memorytool.cpp

HEADERS += \
	SDLMain.h \
    ../../libs/PhTools/memorytool.h

OBJECTIVE_SOURCES += \
	SDLMain.m

mac: INCLUDEPATH += /Library/Frameworks ../../libs
mac: DEPENDPATH += /Library/Frameworks

mac: LIBS += -F/Library/Frameworks/ -framework SDL -framework Cocoa -framework SDL_image -framework SDL_ttf
