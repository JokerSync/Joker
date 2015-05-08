#-------------------------------------------------
#
# Project created by QtCreator 2013-09-16T10:46:10
#
#-------------------------------------------------

TARGET = FormTest

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($$TOP_ROOT/libs/PhTools/PhTools.pri)
include($$TOP_ROOT/libs/PhSync/PhSync.pri)
include($$TOP_ROOT/libs/PhCommonUI/PhCommonUI.pri)

SOURCES += main.cpp\
		FormTestWindow.cpp \
	AboutDialog.cpp

HEADERS  += FormTestWindow.h \
	AboutDialog.h \
	FormTestSettings.h

FORMS    += FormTestWindow.ui \
	AboutDialog.ui

mac {
	# For the plist version
	QMAKE_INFO_PLIST +=  $${TOP_ROOT}/data/test.plist
}

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
