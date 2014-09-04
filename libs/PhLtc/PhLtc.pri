#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT += serialport

HEADERS += \
	$$TOP_ROOT/libs/PhLtc/PhLtcSettings.h \
	$$TOP_ROOT/libs/PhLtc/PhLtcReader.h \
	$$TOP_ROOT/libs/PhLtc/PhLtcWriter.h \
    $$PWD/PhLtcReaderSettings.h

SOURCES += \
	$$TOP_ROOT/libs/PhLtc/PhLtcReader.cpp \
	$$TOP_ROOT/libs/PhLtc/PhLtcWriter.cpp

unix {
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lltc
}

win32 {
	!exists($$(LTC_PATH)) {
		error("You must define LTC_PATH")
	}

	INCLUDEPATH += $$(LTC_PATH)

	HEADERS += $$(LTC_PATH)\ltc.h \
		$$(LTC_PATH)\encoder.h \
		$$(LTC_PATH)\decoder.h \
		$$(LTC_PATH)\timecode.h

	SOURCES += $$(LTC_PATH)\ltc.c \
		$$(LTC_PATH)\encoder.c \
		$$(LTC_PATH)\decoder.c \
		$$(LTC_PATH)\timecode.c
}


