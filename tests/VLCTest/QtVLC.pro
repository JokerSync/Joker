#QT += gui declarative
QT+= widgets
QT += multimedia multimediawidgets

TEMPLATE = app
TARGET = qtvlc

DESTDIR = ./

##########################################################################################
## WARNING: YOU HAVE TO SET ENVIRONMENT VARIABLES VLC_QT_PATH AND VLC_PLUGIN_PATH OR
## IN YOUR ENVIRONMENT OR IN "PROJECT" => "BUILD ENVIRONMENT" LIKE SO :
##
## VLC_QT_PATH = path/to/vlc/
##
## VLC_PLUGIN_PATH = path/to/vlcplugin (on MAC OS is usually vlc.app/Content/MacOs/plugins
##
##
##########################################################################################


# Input
HEADERS += player.h
SOURCES += main.cpp player.cpp

unix {
INCLUDEPATH += /usr/include/GL \
               /usr/include/SDL \
                /usr/include/vlc \
               /usr/include/qt5

LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf -lvlc
#LIBS += -L$$(LEAP_SDK)/lib/x64 -lLeap

}




macx {
DEPENDPATH += . \
                        $$(VLC_QT_PATH)/include/
INCLUDEPATH += . \
                        $$(VLC_QT_PATH)/include/

INCLUDEPATH += 	../../libs \
                /Library/Frameworks/ \
    #$$(LEAP_SDK)/include


LIBS += -L$$PWD/../../dyns/ -lvlc

INCLUDEPATH += $$PWD/../../dyns
DEPENDPATH += $$PWD/../../dyns

        libs.commands += cp -r $${PWD}/../../dyns $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs/ ;
QMAKE_EXTRA_TARGETS += libs
POST_TARGETDEPS += libs

}

