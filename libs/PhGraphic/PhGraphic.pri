QT		+= opengl

HEADERS += \
	../../libs/PhGraphic/PhGraphicView.h \
	../../libs/PhGraphic/PhGraphicContext.h \
	../../libs/PhGraphic/PhGraphicImage.h \
	../../libs/PhGraphic/PhGraphicText.h \
    ../../libs/PhGraphic/PhGraphicTexturedRect.h \
	../../libs/PhGraphic/PhFont.h \
    ../../libs/PhGraphic/PhGraphicObject.h \
    ../../libs/PhGraphic/PhGraphicRect.h \
    ../../libs/PhGraphic/PhGraphicSolidRect.h

SOURCES += \
	../../libs/PhGraphic/PhGraphicView.cpp \
	../../libs/PhGraphic/PhGraphicContext.cpp \
	../../libs/PhGraphic/PhGraphicImage.cpp \
	../../libs/PhGraphic/PhGraphicText.cpp \
    ../../libs/PhGraphic/PhGraphicTexturedRect.cpp \
	../../libs/PhGraphic/PhFont.cpp \
    ../../libs/PhGraphic/PhGraphicObject.cpp \
    ../../libs/PhGraphic/PhGraphicRect.cpp \
    ../../libs/PhGraphic/PhGraphicSolidRect.cpp

# Windows specific
win32 {
#TODO
	LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
	RESOURCES_PATH = $${DESTDIR}
}

# Ubuntu specific
linux {
	INCLUDEPATH +=  /usr/include/GL \
			   /usr/include/SDL

	DEPENDPATH +=   /usr/include/GL \
			   /usr/include/SDL

	LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
	RESOURCES_PATH = .
}


# MacOS specific
mac {
	OBJECTIVE_SOURCES += ../../libs/PhGraphic/SDLMain.m
	OBJECTIVE_HEADERS += ../../libs/PhGraphic/SDLMain.h
	INCLUDEPATH += /Library/Frameworks/
	LIBS += -F/Library/Frameworks
	LIBS += -framework SDL -framework SDL_image -framework SDL_ttf -framework Cocoa
	RESOURCES_PATH = $${TARGET}.app/Contents/MacOS
}


