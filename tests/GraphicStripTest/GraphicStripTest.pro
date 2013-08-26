#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T14:09:50
#
#-------------------------------------------------

TARGET = GraphicStripTest
TEMPLATE = app


QT       += core gui
QT		+= xml
QT		+= opengl

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += $${JOKER_ROOT}/libs

include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)
include($${JOKER_ROOT}/libs/PhStrip/PhStrip.pri)
include($${JOKER_ROOT}/libs/PhGraphic/PhGraphic.pri)
include($${JOKER_ROOT}/libs/PhGraphicStrip/PhGraphicStrip.pri)

#Main app
SOURCES += main.cpp \
	MainController.cpp \
    MainView.cpp \

HEADERS += \
	MainController.h \
    MainView.h \

QMAKE_POST_LINK += echo $${RESOURCES_PATH}
QMAKE_POST_LINK += && cp $${JOKER_ROOT}/data/img/motif-240.png $${RESOURCES_PATH}
QMAKE_POST_LINK += && cp $${JOKER_ROOT}/data/fonts/Bedizen.ttf $${RESOURCES_PATH}
