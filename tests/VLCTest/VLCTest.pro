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

#copylibs.commands += echo "copying to target $${TARGET}"
#copylibs.commands += && echo "copying to target $${TARGET}"
copylibs.commands += mkdir ./$${TARGET}.app/Contents/Frameworks && cp -r /Users/martindelille/dev/libs/VLC/lib/* ./$${TARGET}.app/Contents/Frameworks/ &&
QMAKE_EXTRA_TARGETS += copylibs
POST_TARGETDEPS += copylibs


