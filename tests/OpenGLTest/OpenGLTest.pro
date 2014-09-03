#-------------------------------------------------
#
# Project created by QtCreator 2013-10-03T18:26:17
#
#-------------------------------------------------

TARGET = OpenGLTest

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

QT += opengl

SOURCES += main.cpp\
        OpenGLTestWindow.cpp

HEADERS  += OpenGLTestWindow.h

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
