QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TEMPLATE = app


SOURCES += main.cpp \
	../../libs/PhTools/memorytool.cpp \
    ../../libs/PhGraphic/SDLMain.cpp

#HEADERS += ../../libs/PhTools/memorytool.h

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
        INCLUDEPATH += /Library/Frameworks/
        LIBS += -F/Library/Frameworks
	LIBS += -framework SDL -framework SDL_image -framework SDL_ttf
}

RESOURCES += GraphicTest.qrc
