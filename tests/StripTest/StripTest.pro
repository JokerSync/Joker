#-------------------------------------------------
#
# Project created by QtCreator 2013-07-29T09:40:38
#
#-------------------------------------------------

QT       += core

QT       += gui
QT       += xml

CONFIG   += console
CONFIG   -= app_bundle

TARGET = StripTest

DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += ORG_NAME=\\\"Phonations\\\"

TEMPLATE = app

QMAKE_CXXFLAGS += -g -fprofile-arcs -ftest-coverage -O0
QMAKE_LFLAGS += -g -fprofile-arcs -ftest-coverage  -O0

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhStrip/PhStrip.pri)

INCLUDEPATH += ../../libs

#Main app
SOURCES += main.cpp \

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/strip/test01.detx) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/strip/test01.mos) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/strip/test02.mos) . $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/strip/test03.mos) . $${CS}
