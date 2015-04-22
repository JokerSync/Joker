#-------------------------------------------------
#
# Project created by QtCreator 2013-08-29T12:28:34
#
#-------------------------------------------------

TARGET = SerialTest

VERSION = $$system(date +%y.%m.%d)

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

QT += serialport

include($$TOP_ROOT/libs/PhTools/PhTools.pri)

SOURCES += main.cpp \
	SerialTestWindow.cpp

HEADERS  += SerialTestWindow.h

FORMS    += SerialTestWindow.ui

mac {
	# For the plist version
	QMAKE_INFO_PLIST +=  $${TOP_ROOT}/data/test.plist
}

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
