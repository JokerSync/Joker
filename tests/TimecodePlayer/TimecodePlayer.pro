#-------------------------------------------------
#
# Project created by QtCreator 2013-08-06T16:37:44
#
#-------------------------------------------------

TARGET = TimecodePlayer

VERSION = $$system(date +%y.%m.%d)

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($${TOP_ROOT}/libs/PhTools/PhTools.pri)
include($$TOP_ROOT/libs/PhSync/PhSync.pri)
include($${TOP_ROOT}/libs/PhCommonUI/PhCommonUI.pri)

SOURCES += main.cpp\
		TimecodePlayerWindow.cpp

HEADERS  += TimecodePlayerWindow.h

FORMS +=  TimecodePlayerWindow.ui

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
