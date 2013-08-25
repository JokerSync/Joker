#-------------------------------------------------
#
# Project created by QtCreator 2013-08-01T12:25:30
#
#-------------------------------------------------

QT       += core gui
QT		+= opengl
QT       += core gui
QT		+= xml
QT +=multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4):

TARGET = GraphicTest
TEMPLATE = app

JOKER_ROOT = $${_PRO_FILE_PWD_}/../..

SOURCES += main.cpp\
        MainWindow.cpp \
	GraphicTestView.cpp \
	../../libs/PhGraphic/PhGraphicView.cpp \
	../../libs/PhGraphic/PhGraphicContext.cpp \
	../../libs/PhGraphic/PhGraphicImage.cpp \
	../../libs/PhGraphic/PhGraphicText.cpp \
    ../../libs/PhGraphic/PhGraphicTexturedRect.cpp \
	../../libs/PhGraphic/PhFont.cpp \
    ../../libs/PhTools/PhColor.cpp \
    ../../libs/PhGraphic/PhGraphicObject.cpp \
    ../../libs/PhGraphic/PhGraphicRect.cpp \
    ../../libs/PhGraphic/PhGraphicSolidRect.cpp

HEADERS  += MainWindow.h \
	GraphicTestView.h \
	../../libs/PhGraphic/PhGraphicView.h \
	../../libs/PhGraphic/PhGraphicContext.h \
	../../libs/PhGraphic/PhGraphicImage.h \
	../../libs/PhGraphic/PhGraphicText.h \
    ../../libs/PhGraphic/PhGraphicTexturedRect.h \
	../../libs/PhGraphic/PhFont.h \
    ../../libs/PhTools/PhColor.h \
    ../../libs/PhGraphic/PhGraphicObject.h \
    ../../libs/PhGraphic/PhGraphicRect.h \
    ../../libs/PhGraphic/PhGraphicSolidRect.h

INCLUDEPATH += ../../libs/PhGraphic \
				../../libs/PhTools \
				 /usr/include/GL \
			   /usr/include/SDL \
				../../libs

DEPENDPATH +=  ../../libs/PhGraphic \
				../../libs/PhTools \
				 /usr/include/GL \
			   /usr/include/SDL \
				../../libs

# Windows specific
win32 {
#TODO
	LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
	RESOURCES_PATH = $${DESTDIR}
}

# Ubuntu specific
linux {
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

QMAKE_POST_LINK += echo $${RESOURCES_PATH}
QMAKE_POST_LINK += && cp $${JOKER_ROOT}/data/img/look.png $${RESOURCES_PATH}
QMAKE_POST_LINK += && cp $${JOKER_ROOT}/data/fonts/Bedizen.ttf $${RESOURCES_PATH}



