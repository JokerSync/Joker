QT		+= opengl

HEADERS += \
	../../libs/PhGraphic/PhGraphicView.h \
	../../libs/PhGraphic/PhGraphicImage.h \
	../../libs/PhGraphic/PhGraphicText.h \
    ../../libs/PhGraphic/PhGraphicTexturedRect.h \
	../../libs/PhGraphic/PhFont.h \
    ../../libs/PhGraphic/PhGraphicObject.h \
    ../../libs/PhGraphic/PhGraphicRect.h \
    ../../libs/PhGraphic/PhGraphicSolidRect.h \
	../../libs/PhGraphic/PhGraphicLoop.h

SOURCES += \
	../../libs/PhGraphic/PhGraphicView.cpp \
	../../libs/PhGraphic/PhGraphicImage.cpp \
	../../libs/PhGraphic/PhGraphicText.cpp \
    ../../libs/PhGraphic/PhGraphicTexturedRect.cpp \
	../../libs/PhGraphic/PhFont.cpp \
    ../../libs/PhGraphic/PhGraphicObject.cpp \
    ../../libs/PhGraphic/PhGraphicRect.cpp \
    ../../libs/PhGraphic/PhGraphicSolidRect.cpp \
    ../../libs/PhGraphic/PhGraphicLoop.cpp

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

unix {
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf
}
