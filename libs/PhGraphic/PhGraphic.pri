#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT		+= opengl

HEADERS += \
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
		!exists($$(SDL_TTF_PATH)) {
				error("You must define SDL_TTF_PATH")
		}

		INCLUDEPATH += $$(SDL_PATH)\include
		INCLUDEPATH += $$(SDL_PATH)\include\SDL2 $$(SDL_TTF_PATH)\include
		LIBS += -lmingw32
		LIBS += -L$$(SDL_PATH)\lib -lSDL2Main -lSDL2
		LIBS += -L$$(SDL_TTF_PATH)\lib -lSDL2_ttf

		QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(SDL_PATH)/bin/SDL2.dll) $${RESOURCES_PATH} $${CS}
		QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(SDL_TTF_PATH)/bin/SDL2_ttf.dll) $${RESOURCES_PATH} $${CS}
		QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(SDL_TTF_PATH)/bin/libfreetype-6.dll) $${RESOURCES_PATH} $${CS}
}

# Ubuntu specific
linux {
	INCLUDEPATH +=  /usr/include/GL
	DEPENDPATH +=   /usr/include/GL
	INCLUDEPATH += /usr/include
	LIBS += -L/usr/lib
}

unix {
	LIBS += -lSDL2 -lSDL2_ttf
}
