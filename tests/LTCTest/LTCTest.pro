#-------------------------------------------------
#
# Project created by QtCreator 2013-12-16T16:13:48
#
#-------------------------------------------------
cache()

QT       += core
QT       -= gui

TARGET = LTCTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

unix {
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lltc -lm
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


}

DEFINES += APP_NAME=\\\"$$TARGET\\\"

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..
INCLUDEPATH += $${JOKER_ROOT}/libs
include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)
