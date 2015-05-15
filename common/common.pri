#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TEMPLATE = app

win32 {
	VERSION = $$system($$TOP_ROOT/scripts/date.bat)
}
else {
	VERSION = $$system(date +%y.%m.%d)
}

# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += ORG_NAME=\\\"Phonations\\\"

INCLUDEPATH += $$TOP_ROOT/libs

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

	INCLUDEPATH += $$(BOOST_PATH)
}

# linux specific
linux {
	CS = ;
	RESOURCES_PATH = ./
	DEFINES += PATH_TO_RESSOURCES=\\\"\\\"
}


# MacOS specific
mac {
#	QMAKE_MAC_SDK = macosx10.9
	CS = ;
	RESOURCES_PATH = $${TARGET}.app/Contents/Resources
	DEFINES += PATH_TO_RESSOURCES=\\\"/../Resources/\\\"
}

# MacOS and linux specific
unix {
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib
}
