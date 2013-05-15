TEMPLATE = app
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
	SDLMain.h

OBJECTIVE_SOURCES += \
	SDLMain.m

mac: INCLUDEPATH += /Library/Frameworks
mac: DEPENDPATH += /Library/Frameworks

mac: LIBS += -F/Library/Frameworks/ -framework SDL -framework Cocoa -framework SDL_image -framework SDL_ttf
