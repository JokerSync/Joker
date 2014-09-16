#-------------------------------------------------
#
# Project created by QtCreator 2014-09-01T17:54:58
#
#-------------------------------------------------

QT       += testlib
CONFIG   += console
CONFIG   -= app_bundle

INCLUDEPATH += $$TOP_ROOT/vendor/igloo

include($$TOP_ROOT/libs/PhTools/PhTools.pri)

SOURCES += $$TOP_ROOT/specs/main.cpp
