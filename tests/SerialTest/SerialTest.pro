#-------------------------------------------------
#
# Project created by QtCreator 2013-08-29T12:28:34
#
#-------------------------------------------------

QT       += core gui serialport widgets

TARGET = SerialTest
TEMPLATE = app

VERSION = 1.0.0

include(../../libs/PhTools/PhTools.pri)

SOURCES += main.cpp\
		SerialTestWindow.cpp

HEADERS  += SerialTestWindow.h

FORMS    += SerialTestWindow.ui

mac {
	# For the plist version
	QMAKE_INFO_PLIST +=  $${JOKER_ROOT}/data/test.plist
}
