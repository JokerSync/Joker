#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#
# Project created by QtCreator 2014-09-01T17:54:58
#

QT       += testlib
CONFIG   += console
CONFIG   -= app_bundle


CONFIG(release, debug|release) {
    QMAKE_CXXFLAGS += -g -O0 -fprofile-arcs -ftest-coverage
    QMAKE_LFLAGS += -g -O0 -fprofile-arcs -ftest-coverage
}

INCLUDEPATH += \
    $$TOP_ROOT/vendor/bandit \
    $$PWD

include($$TOP_ROOT/libs/PhTools/PhTools.pri)

SOURCES += $$PWD/main.cpp \
    $$PWD/CommonSpec.cpp

HEADERS += \
    $$PWD/CommonSpec.h \

# Common qmake directive
include($$TOP_ROOT/common/common.pri)
