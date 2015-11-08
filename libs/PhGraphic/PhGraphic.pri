#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT		+= opengl

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
	$$PWD/PhQmlView.h

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
	$$PWD/PhGraphicArrow.cpp \
	$$PWD/PhQmlView.cpp

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
	LIBS += -lSDL2 -lSDL2_image -lSDL2_ttf
}

mac {
	QMAKE_CXXFLAGS += -F/Library/Frameworks
	QMAKE_OBJECTIVE_CFLAGS += -F/Library/Frameworks
	LIBS += -F/Library/Frameworks

	LIBS += -framework SDL2 -framework SDL2_image -framework SDL2_ttf
}

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
	}

}

