TEMPLATE = app

SOURCES += main.cpp

INCLUDEPATH += ../../libs
include(../../libs/PhTools/PhTools.pri);

# Windows specific
win32{
#	INCLUDEPATH += $$(SDL_INCLUDE_PATH)
	RESOURCES_PATH = .
}

# Ubuntu specific
linux {
	INCLUDEPATH += /usr/include/SDL
	LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
	RESOURCES_PATH = .
}

# MacOS specific
mac {
#	SOURCES += ../../libs/PhGraphic/SDLMain.cpp
	OBJECTIVE_HEADERS += ../../libs/PhGraphic/SDLMain.h
	OBJECTIVE_SOURCES += ../../libs/PhGraphic/SDLMain.m
	LIBS += -framework Cocoa

	QMAKE_CXXFLAGS += -F/Library/Frameworks
	QMAKE_OBJECTIVE_CFLAGS += -F/Library/Frameworks
	LIBS += -F/Library/Frameworks

	LIBS += -framework SDL -framework SDL_image -framework SDL_ttf
	RESOURCES_PATH = $${TARGET}.app/Contents/Resources
}

QMAKE_POST_LINK += echo $${RESOURCES_PATH};
QMAKE_POST_LINK += cp $${_PRO_FILE_PWD_}/../../data/img/look.png $${RESOURCES_PATH};
QMAKE_POST_LINK += cp $${_PRO_FILE_PWD_}/../../data/fonts/Bedizen.ttf $${RESOURCES_PATH};

CONFIG(release, debug|release) {

	mac {
		QMAKE_POST_LINK += rm -rf $${TARGET}.app/Contents/Frameworks/;
		QMAKE_POST_LINK += mkdir $${TARGET}.app/Contents/Frameworks;
		QMAKE_POST_LINK += cp -R /Library/Frameworks/SDL.framework $${TARGET}.app/Contents/Frameworks/;
		QMAKE_POST_LINK += cp -R /Library/Frameworks/SDL_image.framework $${TARGET}.app/Contents/Frameworks/;
		QMAKE_POST_LINK += cp -R /Library/Frameworks/SDL_ttf.framework $${TARGET}.app/Contents/Frameworks/;

		QMAKE_POST_LINK += install_name_tool -change @rpath/SDL.framework/Versions/A/SDL  @executable_path/../Frameworks/SDL.framework/Versions/A/SDL $${TARGET}.app/Contents/MacOS/$${TARGET};
		QMAKE_POST_LINK += install_name_tool -change @rpath/SDL_image.framework/Versions/A/SDL_image  @executable_path/../Frameworks/SDL_image.framework/Versions/A/SDL_image $${TARGET}.app/Contents/MacOS/$${TARGET};
		QMAKE_POST_LINK += install_name_tool -change @rpath/SDL_ttf.framework/Versions/A/SDL_ttf @executable_path/../Frameworks/SDL_ttf.framework/Versions/A/SDL_ttf $${TARGET}.app/Contents/MacOS/$${TARGET};

		QMAKE_POST_LINK += macdeployqt $${TARGET}.app -dmg;
	}

}
