#-------------------------------------------------
#
# Project created by QtCreator 2013-11-13T10:56:17
#
#-------------------------------------------------

TARGET = GraphicSyncTest

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($$TOP_ROOT/libs/PhTools/PhTools.pri)
include($$TOP_ROOT/libs/PhSync/PhSync.pri)
include($$TOP_ROOT/libs/PhSony/PhSony.pri)
include($$TOP_ROOT/libs/PhGraphic/PhGraphic.pri)

SOURCES += main.cpp\
		GraphicSyncTestWindow.cpp \
	GraphicSyncTestView.cpp

HEADERS  += GraphicSyncTestWindow.h \
	GraphicSyncTestView.h \
	GraphicSyncTestSettings.h

FORMS    += GraphicSyncTestWindow.ui

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
