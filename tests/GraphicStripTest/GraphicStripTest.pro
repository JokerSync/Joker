#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T14:09:50
#
#-------------------------------------------------

TARGET = GraphicStripTest
TEMPLATE = app

QT += core gui

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += $${JOKER_ROOT}/libs

include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)
include($${JOKER_ROOT}/libs/PhStrip/PhStrip.pri)
include($${JOKER_ROOT}/libs/PhGraphic/PhGraphic.pri)
include($${JOKER_ROOT}/libs/PhGraphicStrip/PhGraphicStrip.pri)

#Main app
SOURCES += main.cpp \
    MainView.cpp \
	MainWindow.cpp \
	Dialog.cpp \

HEADERS += \
    MainView.h \
    MainWindow.h \
    ../../libs/PhStrip/PhStripPeopleObject.h \
	Dialog.h \

FORMS += \
    MainWindow.ui

QMAKE_POST_LINK += echo $${RESOURCES_PATH}
QMAKE_POST_LINK += && cp $${JOKER_ROOT}/data/img/motif-240.png $${RESOURCES_PATH}
QMAKE_POST_LINK += && cp $${JOKER_ROOT}/data/fonts/Bedizen.ttf $${RESOURCES_PATH}

