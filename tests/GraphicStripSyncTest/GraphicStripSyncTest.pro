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
	GraphicStripSyncTestWindow.cpp \
	SonyStripSynchronizer.cpp \
	PreferencesDialog.cpp

HEADERS += \
	GraphicStripSyncTestWindow.h \
	SonyStripSynchronizer.h \
	PreferencesDialog.h \
	GraphicStripSyncTestSettings.h

FORMS += \
	GraphicStripSyncTestWindow.ui \
	PreferencesDialog.ui

QMAKE_INFO_PLIST +=  ../../data/test.plist

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include(../../common/deploy.pri)
