#-------------------------------------------------
#
# Project created by QtCreator 2013-08-01T12:25:30
#
#-------------------------------------------------

TARGET = GraphicTest
TEMPLATE = app

QT	+= core gui

VERSION = 1.0.0

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += $${JOKER_ROOT}/libs

DEFINES += APP_NAME=\\\"$$TARGET\\\"

include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)
include($${JOKER_ROOT}/libs/PhGraphic/PhGraphic.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)

HEADERS  += \
	GraphicTestView.h \
	GraphicTestWindow.h \
    GraphicTestSettings.h

SOURCES += main.cpp \
	GraphicTestView.cpp \
    GraphicTestWindow.cpp

FORMS += \
	GraphicTestWindow.ui

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/look.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/fonts/Bedizen.ttf) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/fonts/SWENSON.TTF) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/motif-240_black.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/motif-240.png) $${RESOURCES_PATH} $${CS}

mac {
	# For the plist version
	QMAKE_INFO_PLIST +=  $${JOKER_ROOT}/data/test.plist
}

CONFIG(release, debug|release) {
	mac {
		QMAKE_POST_LINK += macdeployqt $${TARGET}.app -dmg;
		QMAKE_POST_LINK += cp $${TARGET}.dmg $$(JOKER_RELEASE_PATH)/tools/$${TARGET}_v$${VERSION}.dmg
	}

}

