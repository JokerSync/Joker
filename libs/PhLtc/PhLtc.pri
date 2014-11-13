#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

HEADERS += \
	$$PWD/PhLtcSettings.h \
	$$PWD/PhLtcReader.h \
	$$PWD/PhLtcWriter.h \
	$$PWD/PhLtcReaderSettings.h

SOURCES += \
	$$PWD/PhLtcReader.cpp \
	$$PWD/PhLtcWriter.cpp

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


