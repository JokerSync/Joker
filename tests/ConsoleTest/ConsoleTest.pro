#-------------------------------------------------
#
# Project created by QtCreator 2013-05-27T17:36:08
#
#-------------------------------------------------

QT       += core

TARGET = ConsoleTest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += APP_NAME=\\\"$$TARGET\\\"

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhSync/PhSync.pri)

SOURCES += main.cpp

INCLUDEPATH += 	../../libs

RESOURCES_PATH = .

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include(../../common/deploy.pri)
