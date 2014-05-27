cache()

TEMPLATE = app
TARGET = VideoTest

# The application version
VERSION = 1.0.0
# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += ORG_NAME=\\\"Phonations\\\"

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhGraphic/PhGraphic.pri)
include(../../libs/PhVideo/PhVideo.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)

HEADERS += VideoTestWindow.h \
	VideoTestSettings.h

SOURCES += \
	main.cpp \
	VideoTestWindow.cpp

FORMS += \
	VideoTestWindow.ui

mac {
	# For the plist version
	QMAKE_INFO_PLIST +=  $${JOKER_ROOT}/data/test.plist
}

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include(../../common/deploy.pri)

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/fonts/HelveticaCYPlain.ttf) $${RESOURCES_PATH} $${CS}
