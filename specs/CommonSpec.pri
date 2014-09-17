#-------------------------------------------------
#
# Project created by QtCreator 2014-09-01T17:54:58
#
#-------------------------------------------------

QT       += testlib
CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += $$TOP_ROOT/vendor/igloo \
    $$PWD

include($$TOP_ROOT/libs/PhTools/PhTools.pri)

SOURCES += $$PWD/main.cpp \
    $$PWD/CommonSpec.cpp

HEADERS += \
    $$PWD/CommonSpec.h
