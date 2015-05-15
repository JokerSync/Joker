#-------------------------------------------------
#
# Project created by QtCreator 2014-02-22T23:26:45
#
#-------------------------------------------------

TARGET = TextEditTest

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($$TOP_ROOT/libs/PhTools/PhTools.pri)
include($$TOP_ROOT/libs/PhSync/PhSync.pri)
include($$TOP_ROOT/libs/PhCommonUI/PhCommonUI.pri)

SOURCES += main.cpp\
		TextEditTestWindow.cpp

HEADERS  += TextEditTestWindow.h \
	TextEditTestSettings.h

FORMS    += TextEditTestWindow.ui

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
