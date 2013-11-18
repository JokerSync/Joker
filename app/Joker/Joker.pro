#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T14:09:50
#
#-------------------------------------------------

TARGET = Joker
TEMPLATE = app
ICON = joker.icns

QT += core gui

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..


# The application version
VERSION = 1.0.7
# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += ORG_NAME=\\\"Phonations\\\"

# For the plist version
QMAKE_INFO_PLIST +=  $${JOKER_ROOT}/data/joker.plist
QMAKE_POST_LINK += sed -i -e "s/@VERSION@/$$VERSION/g" "./$${TARGET}.app/Contents/Info.plist";


INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)
include(../../libs/PhStrip/PhStrip.pri)
include(../../libs/PhGraphic/PhGraphic.pri)
include(../../libs/PhGraphicStrip/PhGraphicStrip.pri)
include(../../libs/PhVideo/PhVideo.pri)
include(../../libs/PhSync/PhSync.pri)

#Main app
SOURCES += main.cpp \
	MainWindow.cpp \
	VideoStripView.cpp \
	SonyVideoStripSynchronizer.cpp \
    AboutMenu.cpp \
    PreferencesDialog.cpp \
	PropertyDialog.cpp \


HEADERS += \
    MainWindow.h \
	VideoStripView.h \
	SonyVideoStripSynchronizer.h \
    AboutMenu.h \
    PreferencesDialog.h \
	PropertyDialog.h \

FORMS += \
    MainWindow.ui \
    AboutMenu.ui \
    PreferencesDialog.ui \
	PropertyDialog.ui \

QMAKE_POST_LINK += cp $${JOKER_ROOT}/data/img/joker.png $${RESOURCES_PATH}/../Resources/;


CONFIG(release, debug|release) {

	mac {
		QMAKE_POST_LINK += macdeployqt $${TARGET}.app -dmg;
	}

}
