#-------------------------------------------------
#
# Project created by QtCreator 2014-01-27T17:15:26
#
#-------------------------------------------------

TARGET = LTCTool

VERSION = $$system(date +%y.%m.%d)

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($$TOP_ROOT/libs/PhTools/PhTools.pri);
include($$TOP_ROOT/libs/PhAudio/PhAudio.pri);
include($$TOP_ROOT/libs/PhSync/PhSync.pri);
include($$TOP_ROOT/libs/PhLtc/PhLtc.pri);
include($$TOP_ROOT/libs/PhCommonUI/PhCommonUI.pri);

SOURCES += main.cpp\
	LTCToolWindow.cpp \
	PreferencesDialog.cpp

HEADERS  += LTCToolWindow.h \
	PreferencesDialog.h \
	LTCToolSettings.h

FORMS    += LTCToolWindow.ui \
	PreferencesDialog.ui

mac {
	# For the plist version
	QMAKE_INFO_PLIST +=  $${TOP_ROOT}/data/test.plist
}

PH_DEPLOY_LOCATION = $$(JOKER_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
