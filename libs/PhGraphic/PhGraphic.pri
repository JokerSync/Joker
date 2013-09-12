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
#TODO
	LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
}

# Ubuntu specific
linux {
	INCLUDEPATH +=  /usr/include/GL \
			   /usr/include/SDL

	DEPENDPATH +=   /usr/include/GL \
			   /usr/include/SDL

	LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
}


# MacOS specific
mac {
	SOURCES += ../../libs/PhGraphic/SDLMain.cpp
	INCLUDEPATH += /Library/Frameworks/
	LIBS += -F/Library/Frameworks
	LIBS += -framework SDL -framework SDL_image -framework SDL_ttf

#	OBJECTIVE_SOURCES += ../../libs/PhGraphic/SDLMain.m
#	OBJECTIVE_HEADERS += ../../libs/PhGraphic/SDLMain.h
#	LIBS += -framework Cocoa

#	copylibs.commands += cp -r /Library/Frameworks/SDL* $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs/;
}


