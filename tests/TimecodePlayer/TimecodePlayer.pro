#-------------------------------------------------
#
# Project created by QtCreator 2013-08-06T16:37:44
#
#-------------------------------------------------
cache()
QT       += multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimecodePlayer
TEMPLATE = app

# The application version
VERSION = 1.0.0

DEFINES += APP_NAME=\\\"$$TARGET\\\"

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += $${JOKER_ROOT}/libs

include($${JOKER_ROOT}/libs/PhTools/PhTools.pri)
include(../../libs/PhSync/PhSync.pri)
include($${JOKER_ROOT}/libs/PhCommonUI/PhCommonUI.pri)

SOURCES += main.cpp\
		TimecodePlayerWindow.cpp

HEADERS  += TimecodePlayerWindow.h

FORMS +=  TimecodePlayerWindow.ui

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include(../../common/deploy.pri)
