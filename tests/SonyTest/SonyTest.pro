#-------------------------------------------------
#
# Project created by QtCreator 2013-08-14T20:05:01
#
#-------------------------------------------------

QT       += core gui
QT += widgets

#greaterThan(QT_MAJOR_VERSION, 4):

TARGET = SonyTest

VERSION = 1.0.0

TEMPLATE = app
ICON = sony.icns

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs//PhCommonUI/PhCommonUI.pri)
include(../../libs/PhAudio/PhAudio.pri)
include(../../libs/PhSync/PhSync.pri)

SOURCES += main.cpp\
		SonyTestWindow.cpp

HEADERS  += SonyTestWindow.h \
	SonyTestSettings.h

FORMS    += SonyTestWindow.ui

mac {
	# For the plist version
	QMAKE_INFO_PLIST +=  $${JOKER_ROOT}/data/test.plist
}

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include(../../common/deploy.pri)
