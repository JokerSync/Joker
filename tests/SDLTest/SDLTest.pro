QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TEMPLATE = app


SOURCES += main.cpp \
    ../../libs/PhTools/memorytool.cpp

HEADERS += \
	SDLMain.h \
    ../../libs/PhTools/memorytool.h

INCLUDEPATH += ../../libs

win32{
INCLUDEPATH += C:/Users/Julien/Documents/SDL/SDL/include/

LIBS += -LC:/Users/Julien/Documents/SDL/SDL/lib -llibSDL \
-lSDL_image \
-lSDL_ttf

}

unix {
INCLUDEPATH += /usr/include/SDL
LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
}

mac {
 OBJECTIVE_SOURCES += SDLMain.m
 INCLUDEPATH += /Library/Frameworks/SDL \
               /Library/Frameworks/SDL_image \
                /Library/Frameworks/SDL_ttf
 DEPENDPATH += /Library/Frameworks
 LIBS += -F/Library/Frameworks/ -framework SDL -framework Cocoa -framework SDL_image -framework SDL_ttf
}
