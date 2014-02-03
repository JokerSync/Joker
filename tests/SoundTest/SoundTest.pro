#-------------------------------------------------
#
# Project created by QtCreator 2013-12-13T11:53:51
#
#-------------------------------------------------

QT       += core multimedia widgets

TARGET = SoundTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += APP_NAME=\\\"$$TARGET\\\"

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..
INCLUDEPATH += $${JOKER_ROOT}/libs
include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lltc -lm


SOURCES += main.cpp \
	generator.cpp \
	audioinput.cpp \


CONFIG(release, debug|release) {
	mac {
		QMAKE_POST_LINK += macdeployqt $${TARGET}.app -dmg;
	}

}

HEADERS += \
	generator.h \
	audioinput.h \
