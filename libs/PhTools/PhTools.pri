#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

CONFIG += c++11

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += ORG_NAME=\\\"Phonations\\\"

INCLUDEPATH += ../../libs

HEADERS += \
	../../libs/PhTools/PhTime.h \
	../../libs/PhTools/PhTimeCode.h \
    ../../libs/PhTools/PhClock.h \
    ../../libs/PhTools/PhDebug.h \
    ../../libs/PhTools/PhTickCounter.h \
    ../../libs/PhTools/PhPictureTools.h \
    ../../libs/PhTools/PhFileTool.h \
	../../libs/PhTools/PhGenericSettings.h \
    ../../libs/PhTools/PhTestTools.h

SOURCES += \
    ../../libs/PhTools/PhTimeCode.cpp \
    ../../libs/PhTools/PhClock.cpp \
    ../../libs/PhTools/PhDebug.cpp \
    ../../libs/PhTools/PhTickCounter.cpp \
    ../../libs/PhTools/PhPictureTools.cpp \
    ../../libs/PhTools/PhFileTool.cpp \
	../../libs/PhTools/PhGenericSettings.cpp \
    ../../libs/PhTools/PhTestTools.cpp

# Windows specific
win32 {
	CS = &
	CONFIG(release, debug|release) {
		RESOURCES_PATH = $$shell_path(./release/)
		message(Release mode)
	}
	CONFIG(debug, debug|release) {
		RESOURCES_PATH = $$shell_path(./debug/)
		message(Debug mode)
	}
	DEFINES += PATH_TO_RESSOURCES=\\\"\\\"
}

# Ubuntu specific
linux {
	CS = ;
	RESOURCES_PATH = ./
	DEFINES += PATH_TO_RESSOURCES=\\\"\\\"
}


# MacOS specific
mac {
	CS = ;
	RESOURCES_PATH = $${TARGET}.app/Contents/Resources
	DEFINES += PATH_TO_RESSOURCES=\\\"/../Resources/\\\"
}

DEFINES += APP_NAME=\\\"$$TARGET\\\"

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..
