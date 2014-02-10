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
	INCLUDEPATH +=  /usr/include/GL
	DEPENDPATH +=   /usr/include/GL
}

unix {
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf
}
