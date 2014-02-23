#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T14:09:50
#
#-------------------------------------------------
cache()
TARGET = GraphicStripTest
TEMPLATE = app

QT += core gui

VERSION = 1.0.0

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)
include(../../libs/PhStrip/PhStrip.pri)
include(../../libs/PhGraphic/PhGraphic.pri)
include(../../libs/PhGraphicStrip/PhGraphicStrip.pri)

#Main app
SOURCES += main.cpp \
	GraphicStripTestWindow.cpp \
    StripPropertiesDialog.cpp \
    GenerateDialog.cpp

HEADERS += \
    GraphicStripTestWindow.h \
    StripPropertiesDialog.h \
    GenerateDialog.h \
    GraphicStripTestSettings.h

FORMS += \
    GraphicStripTestWindow.ui \
    StripPropertiesDialog.ui \
    GenerateDialog.ui
