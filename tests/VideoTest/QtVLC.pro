QT += gui declarative

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
DEPENDPATH += . \
                        $$(VLC_QT_PATH)/include/
INCLUDEPATH += . \
                        $$(VLC_QT_PATH)/include/


macx: LIBS += -L$$PWD/../../dyns/ -lvlc

INCLUDEPATH += $$PWD/../../dyns
DEPENDPATH += $$PWD/../../dyns


# Input
HEADERS += player.h
SOURCES += main.cpp player.cpp


macx {
        libs.commands += cp -r $${PWD}/../../dyns $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs/ ;
}
QMAKE_EXTRA_TARGETS += libs
POST_TARGETDEPS += libs
