#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT		+= opengl

CONFIG += phgraphic

PRECOMPILED_HEADERS += \
    $$PWD/PhGraphic.h \

HEADERS += \
    $$PWD/PhGraphic.h \
	$$PWD/PhGraphicSettings.h \
	$$PWD/PhGraphicView.h \
	$$PWD/PhGraphicImage.h \
	$$PWD/PhGraphicText.h \
	$$PWD/PhGraphicTexturedRect.h \
	$$PWD/PhFont.h \
	$$PWD/PhGraphicObject.h \
	$$PWD/PhGraphicRect.h \
	$$PWD/PhGraphicSolidRect.h \
	$$PWD/PhGraphicLoop.h \
	$$PWD/PhGraphicDisc.h \
	$$PWD/PhGraphicDashedLine.h \
	$$PWD/PhGraphicArrow.h \

SOURCES += \
	$$PWD/PhGraphicView.cpp \
	$$PWD/PhGraphicImage.cpp \
	$$PWD/PhGraphicText.cpp \
	$$PWD/PhGraphicTexturedRect.cpp \
	$$PWD/PhFont.cpp \
	$$PWD/PhGraphicObject.cpp \
	$$PWD/PhGraphicRect.cpp \
	$$PWD/PhGraphicSolidRect.cpp \
	$$PWD/PhGraphicLoop.cpp \
	$$PWD/PhGraphicDisc.cpp \
	$$PWD/PhGraphicDashedLine.cpp \
	$$PWD/PhGraphicArrow.cpp

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
		LIBS += -lmingw32
		LIBS += -L$$(SDL_PATH)\lib -lSDL2Main -lSDL2
		LIBS += -L$$(SDL_IMAGE_PATH)\lib -lSDL2_image
		LIBS += -L$$(SDL_TTF_PATH)\lib -lSDL2_ttf

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
	INCLUDEPATH +=  /usr/include/GL
	DEPENDPATH +=   /usr/include/GL
}

linux {
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf
}

mac {
	QMAKE_CXXFLAGS += -F/Library/Frameworks
	QMAKE_OBJECTIVE_CFLAGS += -F/Library/Frameworks
	LIBS += -F/Library/Frameworks

	LIBS += -framework SDL2 -framework SDL2_image -framework SDL2_ttf
}

CONFIG(release, debug|release) {

	mac {
        QMAKE_POST_LINK += mkdir -p $${TARGET}.app/Contents/Frameworks/;
        QMAKE_POST_LINK += cd $${TARGET}.app/Contents/Frameworks/;

        QMAKE_POST_LINK += echo "Create SDL2 framework bundle";

		QMAKE_POST_LINK += mkdir -p SDL2.framework/Versions/A/Resources;
        QMAKE_POST_LINK += cd SDL2.framework;
        QMAKE_POST_LINK += cp /Library/Frameworks/SDL2.framework/Versions/A/SDL2 Versions/A/;
        QMAKE_POST_LINK += cp /Library/Frameworks/SDL2.framework/Versions/A/Resources/Info.plist Versions/A/Resources;
        QMAKE_POST_LINK += ln -s A Versions/Current;
        QMAKE_POST_LINK += ln -s Versions/Current/SDL2 SDL2;
        QMAKE_POST_LINK += ln -s Versions/Current/Resources Resources;
        QMAKE_POST_LINK += install_name_tool -change @rpath/SDL2.framework/Versions/A/SDL2  @executable_path/../Frameworks/SDL2.framework/Versions/A/SDL2 ../../MacOS/$${TARGET};
        QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" Resources/Info.plist;
        QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep . --force;

        QMAKE_POST_LINK += cd ..;

        # Create SDL2_image framework bundle
        QMAKE_POST_LINK += mkdir -p SDL2_image.framework/Versions/A/Resources;
        QMAKE_POST_LINK += cd SDL2_image.framework;
        QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_image.framework/Versions/A/SDL2_image Versions/A/;
        QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_image.framework/Versions/A/Resources/Info.plist Versions/A/Resources;
        QMAKE_POST_LINK += ln -s A Versions/Current;
        QMAKE_POST_LINK += ln -s Versions/Current/SDL2_image SDL2_image;
        QMAKE_POST_LINK += ln -s Versions/Current/Resources Resources;
        QMAKE_POST_LINK += install_name_tool -change @rpath/SDL2_image.framework/Versions/A/SDL2_image  @executable_path/../Frameworks/SDL2_image.framework/Versions/A/SDL2_image ../../MacOS/$${TARGET};
        QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" Resources/Info.plist;
        QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep . --force;
        QMAKE_POST_LINK += cd ..;

        # Create webp framework bundle
        QMAKE_POST_LINK += mkdir -p webp.framework/Versions/A/Resources;
        QMAKE_POST_LINK += cd webp.framework;
        QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_image.framework/Frameworks/webp.framework/Versions/A/webp Versions/A/;
        QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_image.framework/Frameworks/webp.framework/Versions/A/Resources/Info.plist Versions/A/Resources;
        QMAKE_POST_LINK += ln -s A Versions/Current;
        QMAKE_POST_LINK += ln -s Versions/Current/webp webp;
        QMAKE_POST_LINK += ln -s Versions/Current/Resources Resources;
        QMAKE_POST_LINK += install_name_tool -change @rpath/webp.framework/Versions/A/webp  @executable_path/../Frameworks/webp.framework/Versions/A/webp ../SDL2_image.framework/SDL2_image;
        QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" Resources/Info.plist;
        QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep . --force;
        QMAKE_POST_LINK += cd ..;

        # Create SDL2_ttf framework bundle
        QMAKE_POST_LINK += mkdir -p SDL2_ttf.framework/Versions/A/Resources;
        QMAKE_POST_LINK += cd SDL2_ttf.framework;
        QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_ttf.framework/Versions/A/SDL2_ttf Versions/A/;
        QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_ttf.framework/Versions/A/Resources/Info.plist Versions/A/Resources;
        QMAKE_POST_LINK += ln -s A Versions/Current;
        QMAKE_POST_LINK += ln -s Versions/Current/SDL2_ttf SDL2_ttf;
        QMAKE_POST_LINK += ln -s Versions/Current/Resources Resources;
        QMAKE_POST_LINK += install_name_tool -change @rpath/SDL2_ttf.framework/Versions/A/SDL2_ttf  @executable_path/../Frameworks/SDL2_ttf.framework/Versions/A/SDL2_ttf ../../MacOS/$${TARGET};
        QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" Resources/Info.plist;
        QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep . --force;
        QMAKE_POST_LINK += cd ..;

        # Create FreeType framework bundle
        QMAKE_POST_LINK += mkdir -p FreeType.framework/Versions/A/Resources;
        QMAKE_POST_LINK += cd FreeType.framework;
        QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_ttf.framework/Frameworks/FreeType.framework/Versions/A/FreeType Versions/A/;
        QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_ttf.framework/Frameworks/FreeType.framework/Versions/A/Resources/Info.plist Versions/A/Resources;
        QMAKE_POST_LINK += ln -s A Versions/Current;
        QMAKE_POST_LINK += ln -s Versions/Current/FreeType FreeType;
        QMAKE_POST_LINK += ln -s Versions/Current/Resources Resources;
        QMAKE_POST_LINK += install_name_tool -change @rpath/FreeType.framework/Versions/A/FreeType  @executable_path/../Frameworks/FreeType.framework/Versions/A/FreeType ../SDL2_ttf.framework/SDL2_ttf;
        QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" Resources/Info.plist;
        QMAKE_POST_LINK += codesign -s $$(APPLICATION_CERTIFICATE) -i $BUNDLEID --deep . --force;
        QMAKE_POST_LINK += cd ..;

		QMAKE_POST_LINK += cd ../../..;
	}
}

