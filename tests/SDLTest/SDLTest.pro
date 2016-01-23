#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

TARGET = SDLTest

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.0.0

TEMPLATE = app

SOURCES += main.cpp

# Windows specific
win32 {
	!exists($$(SDL_PATH)) {
		error("You must define SDL_PATH")
	}
	!exists($$(SDL_TTF_PATH)) {
		error("You must define SDL_TTF_PATH")
	}

	INCLUDEPATH += $$(SDL_PATH)\include
	INCLUDEPATH += $$(SDL_PATH)\include\SDL2 $$(SDL_TTF_PATH)\include
	LIBS += -L$$(SDL_PATH)\lib -lSDL2Main -lSDL2
	LIBS += -L$$(SDL_TTF_PATH)\lib -lSDL2_ttf
	RESOURCES_PATH = $$shell_path(./debug/)
	CS = &

	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(SDL_PATH)/bin/SDL2.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(SDL_TTF_PATH)/bin/SDL2_ttf.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(SDL_TTF_PATH)/bin/libfreetype-6.dll) $${RESOURCES_PATH} $${CS}
}

# Ubuntu specific
linux {
	RESOURCES_PATH = .
	CS = ;
}

# MacOS specific
mac {
	RESOURCES_PATH = $${TARGET}.app/Contents/Resources/
	DEFINES += PATH_TO_RESSOURCES=\\\"/../Resources/\\\"
	CS = ;

	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib
}

linux {
	INCLUDEPATH += /usr/include
	LIBS += -L/usr/lib
}

unix {
	LIBS += -lSDL2 -lSDL2_ttf
}

QMAKE_POST_LINK += echo $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/fonts/Bedizen.ttf $${RESOURCES_PATH}) $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/fonts/SWENSON.TTF $${RESOURCES_PATH}) $${CS}

CONFIG(release, debug|release) {

	mac {
		QMAKE_POST_LINK += macdeployqt $${TARGET}.app -dmg;
		QMAKE_POST_LINK += cp $${TARGET}.dmg $$(TESTS_RELEASE_PATH)/$${TARGET}_v$${VERSION}.dmg
	}

}

win32 {
	QMAKE_POST_LINK += windeployqt $${RESOURCES_PATH} $${CS}
}
