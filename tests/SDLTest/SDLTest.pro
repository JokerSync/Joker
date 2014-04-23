QT       += core gui widgets

TEMPLATE = app

SOURCES += main.cpp

# The application version
VERSION = 1.0.0
# Define the preprocessor macro to get the application version in our application.
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += ORG_NAME=\\\"Phonations\\\"

# Windows specific
win32 {
	!exists($$(SDL_PATH)) {
		error("You must define SDL_PATH")
	}
	!exists($$(SDL_IMAGE_PATH)) {
		error("You must define SDL_IMAGE_PATH")
	}
	!exists($$(SDL_TTF_PATH)) {
		error("You must define SDL_TTF_PATH")
	}

	INCLUDEPATH += $$(SDL_PATH)\include
	INCLUDEPATH += $$(SDL_PATH)\include\SDL2 $$(SDL_IMAGE_PATH)\include $$(SDL_TTF_PATH)\include
	LIBS += -L$$(SDL_PATH)\lib -lSDL2Main -lSDL2
	LIBS += -L$$(SDL_IMAGE_PATH)\lib -lSDL2_image
	LIBS += -L$$(SDL_TTF_PATH)\lib -lSDL2_ttf
	RESOURCES_PATH = $$shell_path(./debug/)
	CS = &

	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(SDL_PATH)/bin/SDL2.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(SDL_IMAGE_PATH)/bin/SDL2_image.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(SDL_IMAGE_PATH)/bin/libjpeg-9.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(SDL_IMAGE_PATH)/bin/libpng16-16.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(SDL_IMAGE_PATH)/bin/libtiff-5.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(SDL_IMAGE_PATH)/bin/libwebp-4.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(SDL_IMAGE_PATH)/bin/zlib1.dll) $${RESOURCES_PATH} $${CS}
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
	RESOURCES_PATH = $${TARGET}.app/Contents/MacOS/
	CS = ;

	QMAKE_CXXFLAGS += -F/Library/Frameworks
	QMAKE_OBJECTIVE_CFLAGS += -F/Library/Frameworks
	LIBS += -F/Library/Frameworks

	LIBS += -framework SDL2 -framework SDL2_image -framework SDL2_ttf
}

linux {
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf
}

QMAKE_POST_LINK += echo $${RESOURCES_PATH} $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/img/look.png $${RESOURCES_PATH}) $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/fonts/Bedizen.ttf $${RESOURCES_PATH}) $${CS}
QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${_PRO_FILE_PWD_}/../../data/fonts/SWENSON.TTF $${RESOURCES_PATH}) $${CS}

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
		QMAKE_POST_LINK += cp $${TARGET}.dmg $$(TESTS_RELEASE_PATH)/$${TARGET}_v$${VERSION}.dmg
	}

}
