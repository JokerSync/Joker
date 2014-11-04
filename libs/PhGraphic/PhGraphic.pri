#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT		+= opengl

HEADERS += \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicSettings.h \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicView.h \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicImage.h \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicText.h \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicTexturedRect.h \
	$$TOP_ROOT/libs/PhGraphic/PhFont.h \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicObject.h \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicRect.h \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicSolidRect.h \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicLoop.h \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicDisc.h \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicDashedLine.h \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicArrow.h

SOURCES += \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicView.cpp \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicImage.cpp \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicText.cpp \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicTexturedRect.cpp \
	$$TOP_ROOT/libs/PhGraphic/PhFont.cpp \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicObject.cpp \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicRect.cpp \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicSolidRect.cpp \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicLoop.cpp \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicDisc.cpp \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicDashedLine.cpp \
	$$TOP_ROOT/libs/PhGraphic/PhGraphicArrow.cpp

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
		# Create SDL2 framework bundle
		QMAKE_POST_LINK += mkdir -p $${TARGET}.app/Contents/Frameworks/SDL2.framework/Versions/A/Resources;
		QMAKE_POST_LINK += cp /Library/Frameworks/SDL2.framework/Versions/A/SDL2 $${TARGET}.app/Contents/Frameworks/SDL2.framework/Versions/A/;
		QMAKE_POST_LINK += cp /Library/Frameworks/SDL2.framework/Versions/A/Resources/Info.plist $${TARGET}.app/Contents/Frameworks/SDL2.framework/Versions/A/Resources;
		QMAKE_POST_LINK += ln -s A $${TARGET}.app/Contents/Frameworks/SDL2.framework/Versions/Current;
		QMAKE_POST_LINK += ln -s Versions/Current/SDL2 $${TARGET}.app/Contents/Frameworks/SDL2.framework/SDL2;
		QMAKE_POST_LINK += ln -s Versions/Current/Resources $${TARGET}.app/Contents/Frameworks/SDL2.framework/Resources;
		QMAKE_POST_LINK += install_name_tool -change @rpath/SDL2.framework/Versions/A/SDL2  @executable_path/../Frameworks/SDL2.framework/Versions/A/SDL2 $${TARGET}.app/Contents/MacOS/$${TARGET};
		QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/SDL2.framework/Resources/Info.plist;

		# Create SDL2_image framework bundle
		QMAKE_POST_LINK += mkdir -p $${TARGET}.app/Contents/Frameworks/SDL2_image.framework/Versions/A/Resources;
		QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_image.framework/Versions/A/SDL2_image $${TARGET}.app/Contents/Frameworks/SDL2_image.framework/Versions/A/;
		QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_image.framework/Versions/A/Resources/Info.plist $${TARGET}.app/Contents/Frameworks/SDL2_image.framework/Versions/A/Resources;
		QMAKE_POST_LINK += ln -s A $${TARGET}.app/Contents/Frameworks/SDL2_image.framework/Versions/Current;
		QMAKE_POST_LINK += ln -s Versions/Current/SDL2_image $${TARGET}.app/Contents/Frameworks/SDL2_image.framework/SDL2_image;
		QMAKE_POST_LINK += ln -s Versions/Current/Resources $${TARGET}.app/Contents/Frameworks/SDL2_image.framework/Resources;
		QMAKE_POST_LINK += install_name_tool -change @rpath/SDL2_image.framework/Versions/A/SDL2_image  @executable_path/../Frameworks/SDL2_image.framework/Versions/A/SDL2_image $${TARGET}.app/Contents/MacOS/$${TARGET};
		QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/SDL2_image.framework/Resources/Info.plist;

		# Create webp framework bundle
		QMAKE_POST_LINK += mkdir -p $${TARGET}.app/Contents/Frameworks/webp.framework/Versions/A/Resources;
		QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_image.framework/Frameworks/webp.framework/Versions/A/webp $${TARGET}.app/Contents/Frameworks/webp.framework/Versions/A/;
		QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_image.framework/Frameworks/webp.framework/Versions/A/Resources/Info.plist $${TARGET}.app/Contents/Frameworks/webp.framework/Versions/A/Resources;
		QMAKE_POST_LINK += ln -s A $${TARGET}.app/Contents/Frameworks/webp.framework/Versions/Current;
		QMAKE_POST_LINK += ln -s Versions/Current/webp $${TARGET}.app/Contents/Frameworks/webp.framework/webp;
		QMAKE_POST_LINK += ln -s Versions/Current/Resources $${TARGET}.app/Contents/Frameworks/webp.framework/Resources;
		QMAKE_POST_LINK += install_name_tool -change @rpath/webp.framework/Versions/A/webp  @executable_path/../Frameworks/webp.framework/Versions/A/webp $${TARGET}.app/Contents/MacOS/$${TARGET};
		QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/webp.framework/Resources/Info.plist;

		# Create SDL2_ttf framework bundle
		QMAKE_POST_LINK += mkdir -p $${TARGET}.app/Contents/Frameworks/SDL2_ttf.framework/Versions/A/Resources;
		QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_ttf.framework/Versions/A/SDL2_ttf $${TARGET}.app/Contents/Frameworks/SDL2_ttf.framework/Versions/A/;
		QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_ttf.framework/Versions/A/Resources/Info.plist $${TARGET}.app/Contents/Frameworks/SDL2_ttf.framework/Versions/A/Resources;
		QMAKE_POST_LINK += ln -s A $${TARGET}.app/Contents/Frameworks/SDL2_ttf.framework/Versions/Current;
		QMAKE_POST_LINK += ln -s Versions/Current/SDL2_ttf $${TARGET}.app/Contents/Frameworks/SDL2_ttf.framework/SDL2_ttf;
		QMAKE_POST_LINK += ln -s Versions/Current/Resources $${TARGET}.app/Contents/Frameworks/SDL2_ttf.framework/Resources;
		QMAKE_POST_LINK += install_name_tool -change @rpath/SDL2_ttf.framework/Versions/A/SDL2_ttf @executable_path/../Frameworks/SDL2_ttf.framework/Versions/A/SDL2_ttf $${TARGET}.app/Contents/MacOS/$${TARGET};
		QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/SDL2_ttf.framework/Resources/Info.plist;

		# Create FreeType framework bundle
		QMAKE_POST_LINK += mkdir -p $${TARGET}.app/Contents/Frameworks/FreeType.framework/Versions/A/Resources;
		QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_ttf.framework/Frameworks/FreeType.framework/Versions/A/FreeType $${TARGET}.app/Contents/Frameworks/FreeType.framework/Versions/A/;
		QMAKE_POST_LINK += cp /Library/Frameworks/SDL2_ttf.framework/Frameworks/FreeType.framework/Versions/A/Resources/Info.plist $${TARGET}.app/Contents/Frameworks/FreeType.framework/Versions/A/Resources;
		QMAKE_POST_LINK += ln -s A $${TARGET}.app/Contents/Frameworks/FreeType.framework/Versions/Current;
		QMAKE_POST_LINK += ln -s Versions/Current/FreeType $${TARGET}.app/Contents/Frameworks/FreeType.framework/FreeType;
		QMAKE_POST_LINK += ln -s Versions/Current/Resources $${TARGET}.app/Contents/Frameworks/FreeType.framework/Resources;
		QMAKE_POST_LINK += install_name_tool -change @rpath/FreeType.framework/Versions/A/FreeType  @executable_path/../Frameworks/FreeType.framework/Versions/A/FreeType $${TARGET}.app/Contents/MacOS/$${TARGET};
		QMAKE_POST_LINK += plutil -replace CFBundleIdentifier -string "UNDLEID" $${TARGET}.app/Contents/Frameworks/FreeType.framework/Resources/Info.plist;
	}
}

