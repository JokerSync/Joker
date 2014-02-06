QT       += core gui widgets

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
	RESOURCES_PATH = .
}

# MacOS specific
mac {
	RESOURCES_PATH = $${TARGET}.app/Contents/MacOS/
}

unix {
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf
}

QMAKE_POST_LINK += echo $${RESOURCES_PATH};
QMAKE_POST_LINK += cp $${_PRO_FILE_PWD_}/../../data/img/look.png $${RESOURCES_PATH};
QMAKE_POST_LINK += cp $${_PRO_FILE_PWD_}/../../data/fonts/Bedizen.ttf $${RESOURCES_PATH};
QMAKE_POST_LINK += cp $${_PRO_FILE_PWD_}/../../data/fonts/SWENSON.TTF $${RESOURCES_PATH};

CONFIG(release, debug|release) {

	mac {
		QMAKE_POST_LINK += rm -rf $${TARGET}.app/Contents/Frameworks/;
		QMAKE_POST_LINK += mkdir $${TARGET}.app/Contents/Frameworks;
		QMAKE_POST_LINK += cp -R /Library/Frameworks/SDL2.framework $${TARGET}.app/Contents/Frameworks/;
		QMAKE_POST_LINK += cp -R /Library/Frameworks/SDL2_image.framework $${TARGET}.app/Contents/Frameworks/;
		QMAKE_POST_LINK += cp -R /Library/Frameworks/SDL2_ttf.framework $${TARGET}.app/Contents/Frameworks/;

		QMAKE_POST_LINK += install_name_tool -change @rpath/SDL2.framework/Versions/A/SDL2  @executable_path/../Frameworks/SDL2.framework/Versions/A/SDL2 $${TARGET}.app/Contents/MacOS/$${TARGET};
		QMAKE_POST_LINK += install_name_tool -change @rpath/SDL2_image.framework/Versions/A/SDL2_image  @executable_path/../Frameworks/SDL2_image.framework/Versions/A/SDL2_image $${TARGET}.app/Contents/MacOS/$${TARGET};
		QMAKE_POST_LINK += install_name_tool -change @rpath/SDL2_ttf.framework/Versions/A/SDL2_ttf @executable_path/../Frameworks/SDL2_ttf.framework/Versions/A/SDL2_ttf $${TARGET}.app/Contents/MacOS/$${TARGET};

		QMAKE_POST_LINK += macdeployqt $${TARGET}.app -dmg;
	}

}
