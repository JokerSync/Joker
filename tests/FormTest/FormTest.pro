#-------------------------------------------------
#
# Project created by QtCreator 2013-09-16T10:46:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FormTest
TEMPLATE = app

# The application version
VERSION = 1.0.3

mac {
	QMAKE_INFO_PLIST +=  $${JOKER_ROOT}/data/joker.plist
	QMAKE_POST_LINK += sed -i -e "s/@VERSION@/$$VERSION/g" "./$${TARGET}.app/Contents/Info.plist";
}

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)


SOURCES += main.cpp\
        FormTestWindow.cpp \
    AboutDialog.cpp

HEADERS  += FormTestWindow.h \
    AboutDialog.h \
    FormTestSettings.h

FORMS    += FormTestWindow.ui \
    AboutDialog.ui
