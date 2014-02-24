#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T14:09:50
#
#-------------------------------------------------

TARGET = VideoStripTest
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
	VideoStripTestWindow.cpp \
	VideoStripSynchronizer.cpp \
    VideoStripView.cpp

HEADERS += \
    VideoStripTestWindow.h \
	VideoStripSynchronizer.h \
    VideoStripView.h \
    VideoStripTestSettings.h

FORMS += \
    VideoStripTestWindow.ui

QMAKE_POST_LINK += echo $${RESOURCES_PATH};
QMAKE_POST_LINK += cp $${JOKER_ROOT}/data/img/motif-240.png $${RESOURCES_PATH}/../Resources/;
QMAKE_POST_LINK += cp $${JOKER_ROOT}/data/fonts/SWENSON.TTF $${RESOURCES_PATH}/../Resources/;
