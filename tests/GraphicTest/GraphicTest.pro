#-------------------------------------------------
#
# Project created by QtCreator 2013-08-01T12:25:30
#
#-------------------------------------------------

TARGET = GraphicTest
TEMPLATE = app

QT	+= core gui

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += $${JOKER_ROOT}/libs

DEFINES += APP_NAME=\\\"$$TARGET\\\"

include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)
include($${JOKER_ROOT}/libs/PhGraphic/PhGraphic.pri)

HEADERS  += \
	GraphicTestView.h \
	MainWindow.h \

SOURCES += main.cpp \
	GraphicTestView.cpp \
    MainWindow.cpp

FORMS += \
	MainWindow.ui

QMAKE_POST_LINK += echo $${RESOURCES_PATH};
QMAKE_POST_LINK += cp $${JOKER_ROOT}/data/img/look.png $${RESOURCES_PATH};
QMAKE_POST_LINK += cp $${JOKER_ROOT}/data/fonts/Bedizen.ttf $${RESOURCES_PATH};
QMAKE_POST_LINK += cp $${JOKER_ROOT}/data/fonts/SWENSON.TTF $${RESOURCES_PATH};

CONFIG(release, debug|release) {
	mac {
		QMAKE_POST_LINK += macdeployqt $${TARGET}.app -dmg;
	}

}
