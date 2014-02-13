#-------------------------------------------------
#
# Project created by QtCreator 2013-09-16T10:46:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = FormTest
TEMPLATE = app

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..


# The application version
VERSION = 1.0.3
# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += ORG_NAME=\\\"Phonations\\\"


mac {
	QMAKE_INFO_PLIST +=  $${JOKER_ROOT}/data/joker.plist
	QMAKE_POST_LINK += sed -i -e "s/@VERSION@/$$VERSION/g" "./$${TARGET}.app/Contents/Info.plist";
}


INCLUDEPATH += $${JOKER_ROOT}/libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)


SOURCES += main.cpp\
        MainWindow.cpp \
    AboutMenu.cpp

HEADERS  += MainWindow.h \
    AboutMenu.h

FORMS    += MainWindow.ui \
    AboutMenu.ui
