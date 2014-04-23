#-------------------------------------------------
#
# Project created by QtCreator 2013-11-13T10:56:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphicSyncTest
TEMPLATE = app

# The application version
VERSION = 1.0.0
# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += ORG_NAME=\\\"Phonations\\\"


INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhSync/PhSync.pri)
include(../../libs/PhGraphic/PhGraphic.pri)

SOURCES += main.cpp\
        GraphicSyncTestWindow.cpp \
    GraphicSyncTestView.cpp

HEADERS  += GraphicSyncTestWindow.h \
    GraphicSyncTestView.h

FORMS    += GraphicSyncTestWindow.ui

CONFIG(release, debug|release) {

	mac {
		QMAKE_POST_LINK += macdeployqt $${TARGET}.app -dmg;
		QMAKE_POST_LINK += cp $${TARGET}.dmg $$(TESTS_RELEASE_PATH)$${TARGET}_v$${VERSION}.dmg
	}

}
