#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T14:09:50
#
#-------------------------------------------------

TARGET = GraphicStripTest
TEMPLATE = app

QT += core gui

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)
include(../../libs/PhStrip/PhStrip.pri)
include(../../libs/PhGraphic/PhGraphic.pri)
include(../../libs/PhGraphicStrip/PhGraphicStrip.pri)
include(../../libs/PhVideo/PhVideo.pri)

#Main app
SOURCES += main.cpp \
	MainWindow.cpp \
	VideoStripSynchronizer.cpp \
    VideoStripView.cpp

HEADERS += \
    MainWindow.h \
	VideoStripSynchronizer.h \
    VideoStripView.h

FORMS += \
    MainWindow.ui

QMAKE_POST_LINK += echo $${RESOURCES_PATH}
QMAKE_POST_LINK += && cp $${JOKER_ROOT}/data/img/motif-240.png $${RESOURCES_PATH}
QMAKE_POST_LINK += && cp $${JOKER_ROOT}/data/fonts/Bedizen.ttf $${RESOURCES_PATH}

