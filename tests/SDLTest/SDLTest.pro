QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += ../../libs

# Windows specific
win32{
#	INCLUDEPATH += $$(SDL_INCLUDE_PATH)
}

# Ubuntu specific
linux {
	INCLUDEPATH += /usr/include/SDL
	LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
}

# MacOS specific
mac {
#	SOURCES += ../../libs/PhGraphic/SDLMain.cpp \
	OBJECTIVE_HEADERS += SDLMain.h
	OBJECTIVE_SOURCES += SDLMain.m

	INCLUDEPATH += /Library/Frameworks/
	LIBS += -F/Library/Frameworks
	LIBS += -framework SDL -framework SDL_image -framework SDL_ttf -framework Cocoa
}
