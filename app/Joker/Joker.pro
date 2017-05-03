#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#


cache()

TARGET = Joker

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($$TOP_ROOT/libs/PhTools/PhTools.pri)
include($$TOP_ROOT/libs/PhCommonUI/PhCommonUI.pri)
include($$TOP_ROOT/libs/PhStrip/PhStrip.pri)
include($$TOP_ROOT/libs/PhGraphic/PhGraphic.pri)
include($$TOP_ROOT/libs/PhGraphicStrip/PhGraphicStrip.pri)
include($$TOP_ROOT/libs/PhSync/PhSync.pri)

CONFIG += video ltc midi
# Add syn6 if you want to support Synchronos 6 file import
# CONFIG += syn6

win {
CONFIG += sony
}

mac {
CONFIG += sony
}

# Optionnal library component
video {
	DEFINES += USE_VIDEO
	include($$TOP_ROOT/libs/PhVideo/PhVideo.pri)
}
sony {
	DEFINES += USE_SONY
	include($$TOP_ROOT/libs/PhSony/PhSony.pri)
}
ltc {
	DEFINES += USE_LTC
	include($$TOP_ROOT/libs/PhAudio/PhAudio.pri)
	include($$TOP_ROOT/libs/PhLtc/PhLtc.pri)
}
midi {
	DEFINES += USE_MIDI
	include($$TOP_ROOT/libs/PhMidi/PhMidi.pri)
}

SOURCES += main.cpp \
	JokerWindow.cpp \
	AboutDialog.cpp \
	PreferencesDialog.cpp \
	PropertyDialog.cpp \
	PeopleDialog.cpp \
	PeopleEditionDialog.cpp \
	TimeBetweenTwoFeetDialog.cpp \
    SecondScreenWindow.cpp
HEADERS += \
	JokerWindow.h \
	AboutDialog.h \
	PreferencesDialog.h \
	PropertyDialog.h \
	PeopleDialog.h \
	JokerSettings.h \
	PeopleEditionDialog.h \
	TimeBetweenTwoFeetDialog.h \
    SecondScreenWindow.h
FORMS += \
	JokerWindow.ui \
	AboutDialog.ui \
	PreferencesDialog.ui \
	PropertyDialog.ui \
	PeopleDialog.ui \
	PeopleEditionDialog.ui \
	TimeBetweenTwoFeetDialog.ui

mac{
	ICON = joker.icns

#	QMAKE_CFLAGS += -gdwarf-2
#	QMAKE_CXXFLAGS += -gdwarf-2
}

win32 {
	RC_ICONS += "joker.ico"

	OTHER_FILES += Joker.iss
}

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/joker.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/phonationsBlack.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/motif-240.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/img/motif-240_black.png) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/fonts/Cappella-Regular.ttf) $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/fonts/Bookerly-BoldItalic.ttf) $${RESOURCES_PATH} $${CS}

TRANSLATIONS =	\
	ar_MA.ts \
	de_DE.ts \
	es_ES.ts \
	fr_FR.ts \
	fr_AG.ts \
	it_IT.ts \
	pt_PT.ts \

QMAKE_POST_LINK += lrelease $${_PRO_FILE_PWD_}/fr_FR.ts -qm $${RESOURCES_PATH}/fr_FR.qm $${CS}

QMAKE_POST_LINK += echo "Translation ok" $${CS}
PH_DEPLOY_LOCATION = $$(JOKER_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)

cache()

QMAKE_POST_LINK += echo "Joker build ok" $${CS}

RESOURCES += \
	Joker.qrc
