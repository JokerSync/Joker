#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#


cache()

TARGET = Joker
TEMPLATE = app

QT += core gui

# The application version
VERSION = 1.1.13

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += ORG_NAME=\\\"Phonations\\\"

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)
include(../../libs/PhStrip/PhStrip.pri)
include(../../libs/PhGraphic/PhGraphic.pri)
include(../../libs/PhGraphicStrip/PhGraphicStrip.pri)
include(../../libs/PhVideo/PhVideo.pri)
include(../../libs/PhAudio/PhAudio.pri)
include(../../libs/PhSync/PhSync.pri)

#Main app
SOURCES += main.cpp \
	JokerWindow.cpp \
	Synchronizer.cpp \
	AboutDialog.cpp \
	PreferencesDialog.cpp \
	PropertyDialog.cpp \
	PeopleDialog.cpp \
	PeopleEditionDialog.cpp \
    RulerSpaceDialog.cpp

HEADERS += \
	JokerWindow.h \
	Synchronizer.h \
	AboutDialog.h \
	PreferencesDialog.h \
	PropertyDialog.h \
	PeopleDialog.h \
	JokerSettings.h \
	PeopleEditionDialog.h \
    RulerSpaceDialog.h

FORMS += \
	JokerWindow.ui \
	AboutDialog.ui \
	PreferencesDialog.ui \
	PropertyDialog.ui \
	PeopleDialog.ui \
	PeopleEditionDialog.ui \
    RulerSpaceDialog.ui

unix {
	QMAKE_POST_LINK += sed -E -i \"\" -e \"s/\(PROJECT_NUMBER[ ]*=[ ]*\)[^ ]*/\1$$VERSION/\" \"$${JOKER_ROOT}/.doxygen\";
}

mac{
	ICON = joker.icns

	# For the plist version
	QMAKE_INFO_PLIST +=  $${JOKER_ROOT}/data/joker.plist
	QMAKE_POST_LINK += sed -i \"\" -e "s/@VERSION@/$$VERSION/g" "./$${TARGET}.app/Contents/Info.plist";
}

win32 {
	RC_ICONS += "joker.ico"
}

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/joker.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/phonations.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/phonationsBlack.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/motif-240.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/img/motif-240_black.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/fonts/SWENSON.TTF) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/fonts/HelveticaCYPlain.ttf) $${RESOURCES_PATH} $${CS}


TRANSLATIONS =	fr_FR.ts \

QMAKE_POST_LINK += lrelease $${_PRO_FILE_PWD_}/fr_FR.ts -qm $${RESOURCES_PATH}/fr_FR.qm $${CS}

PH_DEPLOY_LOCATION = $$(JOKER_RELEASE_PATH)
include(../../common/deploy.pri)

cache()

OTHER_FILES += \
	JokerSetup.iss
