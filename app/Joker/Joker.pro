#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#


cache()

TARGET = Joker
TEMPLATE = app
ICON = joker.icns

QT += core gui

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..


# The application version
VERSION = 1.0.10
# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += ORG_NAME=\\\"Phonations\\\"

INCLUDEPATH += ../../libs

# Currently LTC works only on Unix system
unix {
	CONFIG += ltc
}

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)
include(../../libs/PhStrip/PhStrip.pri)
include(../../libs/PhGraphic/PhGraphic.pri)
include(../../libs/PhGraphicStrip/PhGraphicStrip.pri)
include(../../libs/PhVideo/PhVideo.pri)
include(../../libs/PhSync/PhSync.pri)

#Main app
SOURCES += main.cpp \
	JokerWindow.cpp \
	VideoStripView.cpp \
	SonyVideoStripSynchronizer.cpp \
    AboutDialog.cpp \
    PreferencesDialog.cpp \
	PropertyDialog.cpp \
    PeopleDialog.cpp


HEADERS += \
    JokerWindow.h \
	VideoStripView.h \
	SonyVideoStripSynchronizer.h \
    AboutDialog.h \
    PreferencesDialog.h \
	PropertyDialog.h \
    PeopleDialog.h

FORMS += \
    JokerWindow.ui \
    AboutDialog.ui \
    PreferencesDialog.ui \
	PropertyDialog.ui \
	PeopleDialog.ui

mac{
	PATH = "/../Resources"
	QMAKE_POST_LINK += cp $${JOKER_ROOT}/data/img/joker.png $${RESOURCES_PATH};

# For the plist version
	QMAKE_INFO_PLIST +=  $${JOKER_ROOT}/data/joker.plist
	QMAKE_POST_LINK += sed -i \"\" -e "s/@VERSION@/$$VERSION/g" "./$${TARGET}.app/Contents/Info.plist";
warning("I don't like it, see issue 72")
#	QMAKE_POST_LINK += cd $${JOKER_ROOT} && sed -E -i \"\" -e \"s/\(PROJECT_NUMBER[ ]*=[ ]*\)[^ ]*/\1$$VERSION/\" \".doxygen\" && ./scripts/doxygen.sh && cd $${OUT_PWD};
}
win32 {
	PATH = .
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/joker.png) $${RESOURCES_PATH} $${CS}
}


DEFINES += PATH_TO_RESSOURCES=\\\"$$PATH\\\"

TRANSLATIONS =	fr_FR.ts \
				en_US.ts \

QMAKE_POST_LINK += lrelease $${_PRO_FILE_PWD_}/fr_FR.ts -qm $${RESOURCES_PATH}/fr_FR.qm $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/joker.png) $$shell_path($${RESOURCES_PATH}/) $${CS}

CONFIG(release, debug|release) {

	mac {
		QMAKE_POST_LINK += macdeployqt $${TARGET}.app -dmg;
		QMAKE_POST_LINK += cp $${TARGET}.dmg $$(JOKER_RELEASE_PATH)$${TARGET}_v$${VERSION}.dmg
	}

}

cache()
