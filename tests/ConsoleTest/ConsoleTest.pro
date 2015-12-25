#-------------------------------------------------
#
# Project created by QtCreator 2013-05-27T17:36:08
#
#-------------------------------------------------

TARGET = ConsoleTest
CONFIG   += console
CONFIG   -= app_bundle

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($$TOP_ROOT/libs/PhTools/PhTools.pri)

SOURCES += main.cpp

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
