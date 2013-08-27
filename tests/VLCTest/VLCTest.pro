TEMPLATE = app
TARGET = VLCTest
DEPENDPATH += .
QT += widgets core gui
INCLUDEPATH += .
INCLUDEPATH += /Users/martindelille/dev/libs/VLC/include
DEPENDPATH += /Users/martindelille/dev/libs/VLC/include
LIBS += -L/Users/martindelille/dev/libs/VLC/lib/ -lvlc

# Input
HEADERS += player.h
SOURCES += main.cpp player.cpp

DESTDIR = ./


# Windows specific
win32{
}

# Ubuntu specific
linux {

}

# MacOS specific
mac {
VLC_PATH = /Applications/VLC.app/Contents/MacOS/
}

QMAKE_POST_LINK += mkdir -p ./$${TARGET}.app/Contents/MacOS/lib
QMAKE_POST_LINK += && mkdir -p ./$${TARGET}.app/Contents/MacOS/plugins
QMAKE_POST_LINK += && cp -r $${VLC_PATH}/lib/* ./$${TARGET}.app/Contents/MacOS/lib
QMAKE_POST_LINK += && cp -r $${VLC_PATH}/plugins/* ./$${TARGET}.app/Contents/MacOS/plugins


