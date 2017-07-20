#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TEMPLATE = app

win32 {
	PH_DATE = $$system($$TOP_ROOT/scripts/date.bat)
}
else {
	PH_DATE = $$system(date +%y.%m.%d)
}

# Use default icon if not defined
isEmpty(ICON) {
	ICON = $$TOP_ROOT/data/icon.icns
}

VERSION = $$system(git describe --abbrev=0 --tags)
BUILD = $$system(git rev-list $${VERSION}.. --count)

# append build number to version tag
VERSION = $${VERSION}.$${BUILD}

PH_GIT_BRANCH = $$system(git rev-parse --abbrev-ref HEAD)
PH_GIT_REVISION = $$system(git rev-parse HEAD)
if(equals(PH_GIT_BRANCH, "master") || equals(PH_GIT_BRANCH, "HEAD")) {
	PH_FULL_VERSION = $$system(git describe --tags --dirty)
} else {
	PH_FULL_VERSION = $$system(git describe --tags --dirty)_$$PH_GIT_BRANCH
}

message("Version: $${VERSION} build $${BUILD} branch: $${PH_GIT_BRANCH}")

# Define the preprocessor macro to get the application version in our application.
DEFINES += PH_VERSION=\\\"$$VERSION\\\"
DEFINES += PH_BUILD=\\\"$$BUILD\\\"
DEFINES += PH_GIT_BRANCH=\\\"$$PH_GIT_BRANCH\\\"
DEFINES += PH_FULL_VERSION=\\\"$$PH_FULL_VERSION\\\"
DEFINES += PH_APP_NAME=\\\"$$TARGET\\\"
DEFINES += PH_ORG_NAME=\\\"Phonations\\\"
DEFINES += PH_GIT_REVISION=\\\"$$PH_GIT_REVISION\\\"

INCLUDEPATH += $$TOP_ROOT/libs

RESOURCES_PATH = ./
DEFINES += PATH_TO_RESSOURCES=\\\"\\\"

# Windows specific
win32 {
	CS = &&
	CONFIG(release, debug|release) {
		RESOURCES_PATH = $$shell_path(./release/)
	}
	CONFIG(debug, debug|release) {
		RESOURCES_PATH = $$shell_path(./debug/)
	}

	INCLUDEPATH += $$(BOOST_PATH)
}

# MacOS specific
mac {
	app_bundle {
		RESOURCES_PATH = $${TARGET}.app/Contents/Resources
		DEFINES -= PATH_TO_RESSOURCES=\\\"\\\"
		DEFINES += PATH_TO_RESSOURCES=\\\"/../Resources/\\\"
	}
}

# MacOS and linux specific
unix {
	CS = ;
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib
}
