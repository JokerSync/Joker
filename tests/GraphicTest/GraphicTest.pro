#-------------------------------------------------
#
# Project created by QtCreator 2013-08-01T12:25:30
#
#-------------------------------------------------

TARGET = GraphicTest

VERSION = 2.0.0

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($${TOP_ROOT}/libs/PhTools/PhTools.pri)
include($${TOP_ROOT}/libs/PhGraphic/PhGraphic.pri)
include($$TOP_ROOT/libs/PhSync/PhSync.pri)
include($$TOP_ROOT/libs/PhCommonUI/PhCommonUI.pri)

HEADERS  += \
	GraphicTestWindow.h \
	GraphicTestSettings.h

SOURCES += main.cpp \
	GraphicTestWindow.cpp

FORMS += \
	GraphicTestWindow.ui

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/box.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/fonts/Bedizen.ttf) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/fonts/SWENSON.TTF) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/motif-240_black.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/motif-240.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/fonts/Helvetica.ttf) $${RESOURCES_PATH} $${CS}

win32 {
	OTHER_FILES += GraphicTest.iss
}

PH_DEPLOY_LOCATION = $$(TEST_RELEASE_PATH)

include($$TOP_ROOT/common/deploy.pri)
