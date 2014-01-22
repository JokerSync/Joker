#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T14:09:50
#
#-------------------------------------------------

TARGET = GraphicStripTest
TEMPLATE = app

QT += core gui

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)
include(../../libs/PhStrip/PhStrip.pri)
include(../../libs/PhGraphic/PhGraphic.pri)
include(../../libs/PhGraphicStrip/PhGraphicStrip.pri)

#Main app
SOURCES += main.cpp \
	MainWindow.cpp \
    StripPropertiesDialog.cpp \
    GenerateDialog.cpp

HEADERS += \
    MainWindow.h \
    StripPropertiesDialog.h \
    GenerateDialog.h

FORMS += \
    MainWindow.ui \
    StripPropertiesDialog.ui \
    GenerateDialog.ui


mac{
	PATH = "/../Resources/"
}
win32{
	RESOURCES_PATH = $$shell_path(./debug/)
}


DEFINES += PATH_TO_RESSOURCES=\\\"$$PATH\\\"
