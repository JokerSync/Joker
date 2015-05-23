#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#


cache()

TARGET = Joker

CONFIG += video

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($$TOP_ROOT/libs/PhTools/PhTools.pri)
include($$TOP_ROOT/libs/PhCommonUI/PhCommonUI.pri)
include($$TOP_ROOT/libs/PhStrip/PhStrip.pri)
include($$TOP_ROOT/libs/PhGraphic/PhGraphic.pri)
include($$TOP_ROOT/libs/PhGraphicStrip/PhGraphicStrip.pri)

video {
DEFINES += USE_VIDEO
include($$TOP_ROOT/libs/PhVideo/PhVideo.pri)
}
include($$TOP_ROOT/libs/PhAudio/PhAudio.pri)
include($$TOP_ROOT/libs/PhSync/PhSync.pri)
include($$TOP_ROOT/libs/PhSony/PhSony.pri)
include($$TOP_ROOT/libs/PhLtc/PhLtc.pri)
include($$TOP_ROOT/libs/PhMidi/PhMidi.pri)

SOURCES += main.cpp \
	JokerWindow.cpp \
	AboutDialog.cpp \
	PreferencesDialog.cpp \
	PropertyDialog.cpp \
	PeopleDialog.cpp \
	PeopleEditionDialog.cpp \
	TimeBetweenTwoFeetDialog.cpp

HEADERS += \
	JokerWindow.h \
	AboutDialog.h \
	PreferencesDialog.h \
	PropertyDialog.h \
	PeopleDialog.h \
	JokerSettings.h \
	PeopleEditionDialog.h \
	TimeBetweenTwoFeetDialog.h

FORMS += \
	JokerWindow.ui \
	AboutDialog.ui \
	PreferencesDialog.ui \
	PropertyDialog.ui \
	PeopleDialog.ui \
	PeopleEditionDialog.ui \
	TimeBetweenTwoFeetDialog.ui

unix {
	QMAKE_POST_LINK += sed -E -i \"\" -e \"s/\(PROJECT_NUMBER[ ]*=[ ]*\)[^ ]*/\1$$VERSION/\" \"$${TOP_ROOT}/.doxygen\";
}

mac{
	ICON = joker.icns

	# For the plist version
	OTHER_FILES += $${TOP_ROOT}/data/joker.plist

	QMAKE_INFO_PLIST +=  $${TOP_ROOT}/data/joker.plist
	QMAKE_POST_LINK += sed -i \"\" -e "s/@VERSION@/$$VERSION/g" "./$${TARGET}.app/Contents/Info.plist";
}

win32 {
	RC_ICONS += "joker.ico"

	OTHER_FILES += JokerSetup.iss
}

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/joker.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/phonations.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/phonationsBlack.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/motif-240.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/motif-240_black.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/fonts/SWENSON.TTF) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/fonts/Helvetica.ttf) $${RESOURCES_PATH} $${CS}


TRANSLATIONS =	fr_FR.ts \

QMAKE_POST_LINK += lrelease $${_PRO_FILE_PWD_}/fr_FR.ts -qm $${RESOURCES_PATH}/fr_FR.qm $${CS}

PH_DEPLOY_LOCATION = $$(JOKER_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)

cache()
