#-------------------------------------------------
#
# Project created by QtCreator 2014-01-27T17:15:26
#
#-------------------------------------------------

TARGET = AudioTest

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($$TOP_ROOT/libs/PhTools/PhTools.pri);
include($$TOP_ROOT/libs/PhAudio/PhAudio.pri);

SOURCES += main.cpp\
    AudioTestWindow.cpp \
    PreferencesDialog.cpp \
    AudioTestWriter.cpp

HEADERS  += AudioTestWindow.h \
    PreferencesDialog.h \
    AudioTestWriter.h

FORMS    += AudioTestWindow.ui \
    PreferencesDialog.ui

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
