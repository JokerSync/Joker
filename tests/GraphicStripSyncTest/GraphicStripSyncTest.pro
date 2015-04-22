#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T14:09:50
#
#-------------------------------------------------
cache()
TARGET = GraphicStripSyncTest

VERSION = $$system(date +%y.%m.%d)

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($$TOP_ROOT/libs/PhTools/PhTools.pri)
include($$TOP_ROOT/libs/PhCommonUI/PhCommonUI.pri)
include($$TOP_ROOT/libs/PhAudio/PhAudio.pri)
include($$TOP_ROOT/libs/PhSync/PhSync.pri)
include($$TOP_ROOT/libs/PhSony/PhSony.pri)
include($$TOP_ROOT/libs/PhStrip/PhStrip.pri)
include($$TOP_ROOT/libs/PhGraphic/PhGraphic.pri)
include($$TOP_ROOT/libs/PhGraphicStrip/PhGraphicStrip.pri)

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

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/motif-240.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/motif-240_black.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/fonts/SWENSON.TTF) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/fonts/Helvetica.ttf) $${RESOURCES_PATH} $${CS}

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
