#-------------------------------------------------
#
# Project created by QtCreator 2013-07-29T09:40:38
#
#-------------------------------------------------

CONFIG   += console
CONFIG   -= app_bundle

TARGET = StripTest

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

include($$TOP_ROOT/libs/PhTools/PhTools.pri)
include($$TOP_ROOT/libs/PhSync/PhSync.pri)
include($$TOP_ROOT/libs/PhStrip/PhStrip.pri)

#Main app
SOURCES += main.cpp \

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/strip/test01.detx) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/strip/test01.mos) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/strip/test02.mos) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/strip/test03.mos) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/strip/notitle.detx) . $${CS}

HEADERS += \
    StripTestSettings.h

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
