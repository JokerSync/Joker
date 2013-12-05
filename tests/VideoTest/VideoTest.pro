TEMPLATE = app
TARGET = VideoTest

# The application version
VERSION = 1.0.0
# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += ORG_NAME=\\\"Phonations\\\"

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

INCLUDEPATH += $${JOKER_ROOT}/libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhGraphic/PhGraphic.pri)
include(../../libs/PhVideo/PhVideo.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)

HEADERS += MainView.h

SOURCES += \
    main.cpp \
    MainView.cpp

FORMS += \
    MainView.ui

CONFIG(release, debug|release) {

	mac {
		QMAKE_POST_LINK += macdeployqt $${TARGET}.app -dmg;
		QMAKE_POST_LINK += cp $${TARGET}.dmg $$(JOKER_RELEASE_PATH)$${TARGET}_v$${VERSION}.dmg
	}
}
