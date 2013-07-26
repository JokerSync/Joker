QT       += core

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TEMPLATE = app


SOURCES += main.cpp \
    ../../libs/PhTools/memorytool.cpp

HEADERS += ../../libs/PhTools/memorytool.h

INCLUDEPATH += ../../libs


INCLUDEPATH += $$(SDL_INCLUDE_PATH)

LIBS += -L$$(SDL_LIB_PATH) -llibSDL \
-lSDL_image \
-lSDL_ttf

# Windows specific
win32{
}

# Ubuntu specific
unix {
#INCLUDEPATH += /usr/include/SDL
#LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
}

# MacOS specific
mac {
HEADERS += SDLMain.h
 OBJECTIVE_SOURCES += SDLMain.m
# INCLUDEPATH += /Library/Frameworks/SDL \
#               /Library/Frameworks/SDL_image \
#                /Library/Frameworks/SDL_ttf
 DEPENDPATH += /Library/Frameworks
 LIBS += -F/Library/Frameworks/ -framework Cocoa
# LIBS += -framework SDL -framework SDL_image -framework SDL_ttf
}
