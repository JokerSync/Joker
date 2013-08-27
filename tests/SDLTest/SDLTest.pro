QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += ../../libs

# Windows specific
win32{
#	INCLUDEPATH += $$(SDL_INCLUDE_PATH)
	RESOURCES_PATH = .
}

# Ubuntu specific
linux {
	INCLUDEPATH += /usr/include/SDL
	LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
	RESOURCES_PATH = .
}

# MacOS specific
mac {
#	SOURCES += ../../libs/PhGraphic/SDLMain.cpp
	OBJECTIVE_HEADERS += ../../libs/PhGraphic/SDLMain.h
	OBJECTIVE_SOURCES += ../../libs/PhGraphic/SDLMain.m
	LIBS += -framework Cocoa

	INCLUDEPATH += /Library/Frameworks/
	LIBS += -F/Library/Frameworks
	LIBS += -framework SDL -framework SDL_image -framework SDL_ttf
	RESOURCES_PATH = $${TARGET}.app/Contents/MacOS
}

QMAKE_POST_LINK += echo $${RESOURCES_PATH}
QMAKE_POST_LINK += && cp $${_PRO_FILE_PWD_}/../../data/img/look.png $${RESOURCES_PATH}
QMAKE_POST_LINK += && cp $${_PRO_FILE_PWD_}/../../data/fonts/Bedizen.ttf $${RESOURCES_PATH}
