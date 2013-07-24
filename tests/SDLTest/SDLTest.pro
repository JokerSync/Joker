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


#C:/Users/Julien/Documents/SDL/SDL2-2.0.0/i686-w64-mingw32/include/SDL2 \
#                C:/Users/Julien/Documents/SDL/SDL_ttf-2.0.10/include \
#                C:/Users/Julien/Documents/SDL/SDL2_image-2.0.0/i686-w64-mingw32/include/SDL2

LIBS += -LC:/Users/Julien/Documents/SDL/SDL/lib \
-llibSDL \
-llibSDL2_image \
-lSDL_ttf \
#-llibSDLmain


#-L/C:/Users/Julien/Documents/SDL/SDL2-2.0.0/i686-w64-mingw32/lib/libSDL2.dll.a \
  #  C:/Users/Julien/Documents/SDL/SDL2-2.0.0/i686-w64-mingw32/lib/libSDL2.a \
   # C:/Users/Julien/Documents/SDL/SDL2-2.0.0/i686-w64-mingw32/lib/libSDL2.la \
#       -L/C:/Users/Julien/Documents/SDL/SDL_ttf-2.0.10/lib/SDL_ttf.lib \
#       -L/C:/Users/Julien/Documents/SDL/SDL2_image-2.0.0/i686-w64-mingw32/lib/SDL2_image.dll.a
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
