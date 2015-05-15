#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T14:09:50
#
#-------------------------------------------------
cache()
TARGET = GraphicStripTest

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($$TOP_ROOT/libs/PhTools/PhTools.pri)
include($$TOP_ROOT/libs/PhCommonUI/PhCommonUI.pri)
include($$TOP_ROOT/libs/PhSync/PhSync.pri)
include($$TOP_ROOT/libs/PhStrip/PhStrip.pri)
include($$TOP_ROOT/libs/PhGraphic/PhGraphic.pri)
include($$TOP_ROOT/libs/PhGraphicStrip/PhGraphicStrip.pri)

#Main app
SOURCES += main.cpp \
	GraphicStripTestWindow.cpp \
	StripPropertiesDialog.cpp \
	GenerateDialog.cpp

HEADERS += \
	GraphicStripTestWindow.h \
	StripPropertiesDialog.h \
	GenerateDialog.h \
	GraphicStripTestSettings.h

FORMS += \
	GraphicStripTestWindow.ui \
	StripPropertiesDialog.ui \
	GenerateDialog.ui


QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/motif-240.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/motif-240_black.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/fonts/SWENSON.TTF) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/fonts/Helvetica.ttf) $${RESOURCES_PATH} $${CS}


mac {
	# For the plist version
	QMAKE_INFO_PLIST +=  $${TOP_ROOT}/data/test.plist
}

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
