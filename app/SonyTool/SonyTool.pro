#-------------------------------------------------
#
# Project created by QtCreator 2013-08-14T20:05:01
#
#-------------------------------------------------

TARGET = SonyTool

VERSION = 2.0.0

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($$TOP_ROOT/libs/PhTools/PhTools.pri)
include($$TOP_ROOT/libs//PhCommonUI/PhCommonUI.pri)
include($$TOP_ROOT/libs/PhAudio/PhAudio.pri)
include($$TOP_ROOT/libs/PhSync/PhSync.pri)
include($$TOP_ROOT/libs/PhSony/PhSony.pri)

SOURCES += main.cpp\
		SonyToolWindow.cpp \
    PreferencesDialog.cpp

HEADERS  += SonyToolWindow.h \
	SonyToolSettings.h \
    PreferencesDialog.h

FORMS    += SonyToolWindow.ui \
    PreferencesDialog.ui

mac {
	ICON = sony.icns
}

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
