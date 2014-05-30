#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T14:09:50
#
#-------------------------------------------------
cache()
TARGET = GraphicStripSyncTest
TEMPLATE = app

QT += core gui

VERSION = 1.0.0

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)
include(../../libs/PhAudio/PhAudio.pri)
include(../../libs/PhSync/PhSync.pri)
include(../../libs/PhStrip/PhStrip.pri)
include(../../libs/PhGraphic/PhGraphic.pri)
include(../../libs/PhGraphicStrip/PhGraphicStrip.pri)

#Main app
SOURCES += main.cpp \
	../GraphicStripTest/GraphicStripTestWindow.cpp \
	../GraphicStripTest/StripPropertiesDialog.cpp \
	../GraphicStripTest/GenerateDialog.cpp \
	GraphicStripSyncTestWindow.cpp \
	SonyStripSynchronizer.cpp

HEADERS += \
	../GraphicStripTest/GraphicStripTestWindow.h \
	../GraphicStripTest/StripPropertiesDialog.h \
	../GraphicStripTest/GenerateDialog.h \
	GraphicStripSyncTestWindow.h \
	SonyStripSynchronizer.h \
	GraphicStripSyncTestSettings.h

FORMS += \
	../GraphicStripTest/GraphicStripTestWindow.ui \
	../GraphicStripTest/StripPropertiesDialog.ui \
	../GraphicStripTest/GenerateDialog.ui

QMAKE_INFO_PLIST +=  ../../data/test.plist

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/motif-240.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/motif-240_black.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/fonts/SWENSON.TTF) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/fonts/HelveticaCYPlain.ttf) $${RESOURCES_PATH} $${CS}

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include(../../common/deploy.pri)
