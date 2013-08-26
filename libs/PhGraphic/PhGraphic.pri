QT		+= opengl

HEADERS += \
	$${JOKER_ROOT}/libs/PhGraphic/PhGraphicView.h \
	$${JOKER_ROOT}/libs/PhGraphic/PhGraphicContext.h \
	$${JOKER_ROOT}/libs/PhGraphic/PhGraphicImage.h \
	$${JOKER_ROOT}/libs/PhGraphic/PhGraphicText.h \
    $${JOKER_ROOT}/libs/PhGraphic/PhGraphicTexturedRect.h \
	$${JOKER_ROOT}/libs/PhGraphic/PhFont.h \
    $${JOKER_ROOT}/libs/PhGraphic/PhGraphicObject.h \
    $${JOKER_ROOT}/libs/PhGraphic/PhGraphicRect.h \
    $${JOKER_ROOT}/libs/PhGraphic/PhGraphicSolidRect.h

SOURCES += \
	$${JOKER_ROOT}/libs/PhGraphic/PhGraphicView.cpp \
	$${JOKER_ROOT}/libs/PhGraphic/PhGraphicContext.cpp \
	$${JOKER_ROOT}/libs/PhGraphic/PhGraphicImage.cpp \
	$${JOKER_ROOT}/libs/PhGraphic/PhGraphicText.cpp \
    $${JOKER_ROOT}/libs/PhGraphic/PhGraphicTexturedRect.cpp \
	$${JOKER_ROOT}/libs/PhGraphic/PhFont.cpp \
    $${JOKER_ROOT}/libs/PhGraphic/PhGraphicObject.cpp \
    $${JOKER_ROOT}/libs/PhGraphic/PhGraphicRect.cpp \
    $${JOKER_ROOT}/libs/PhGraphic/PhGraphicSolidRect.cpp

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
#	OBJECTIVE_SOURCES += $${JOKER_ROOT}/libs/PhGraphic/SDLMain.m
#	OBJECTIVE_HEADERS += $${JOKER_ROOT}/libs/PhGraphic/SDLMain.h
	SOURCES += $${JOKER_ROOT}/libs/PhGraphic/SDLMain.cpp
	INCLUDEPATH += /Library/Frameworks/
	LIBS += -F/Library/Frameworks
	LIBS += -framework SDL -framework SDL_image -framework SDL_ttf
#	LIBS += -framework Cocoa
#	copylibs.commands += cp -r /Library/Frameworks/SDL* $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs/;
}


