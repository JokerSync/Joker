#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T14:09:50
#
#-------------------------------------------------

QT       += core
QT		+= xml
QT		+= opengl

QT       += gui

TARGET = StripTest
CONFIG   += console

DESTDIR = ./

TEMPLATE = app


#Main app
SOURCES += main.cpp \
	StripWindow.cpp

HEADERS += StripWindow.h \

#PhStrip
SOURCES += \
	../../libs/PhStrip/PhStripDoc.cpp \
    ../../libs/PhStrip/PhStripObject.cpp \
    ../../libs/PhStrip/PhStripCut.cpp \
    ../../libs/PhStrip/PhStripText.cpp \
    ../../libs/PhStrip/PhStripLoop.cpp \
    ../../libs/PhStrip/PhPeople.cpp \

HEADERS += \
    ../../libs/PhStrip/PhStripObject.h \
    ../../libs/PhStrip/PhStripCut.h \
    ../../libs/PhStrip/PhStripText.h \
    ../../libs/PhStrip/PhStripLoop.h \
    ../../libs/PhStrip/PhPeople.h \
	../../libs/PhStrip/PhStripDoc.h \


#PhGraphics
SOURCES += \
	../../libs/PhGraphic/SDLMain.cpp \
	../../libs/PhGraphic/PhGraphicView.cpp \
	../../libs/PhGraphic/PhGraphicText.cpp \
	../../libs/PhGraphic/PhGraphicObject.cpp \
	../../libs/PhGraphic/PhGraphicImage.cpp \
	../../libs/PhGraphic/PhGraphicRect.cpp \
	../../libs/PhGraphic/PhGraphicTexturedRect.cpp \
	../../libs/PhGraphic/PhGraphicContext.cpp \
	../../libs/PhGraphic/MainWindow.cpp\

HEADERS  += \
	../../libs/PhGraphic/PhGraphicView.h \
	../../libs/PhGraphic/PhGraphicText.h \
	../../libs/PhGraphic/PhGraphicObject.h \
	../../libs/PhGraphic/PhGraphicImage.h \
	../../libs/PhGraphic/PhGraphicRect.h \
	../../libs/PhGraphic/PhGraphicTexturedRect.h \
	../../libs/PhGraphic/PhGraphicContext.h \
	../../libs/PhGraphic/MainWindow.h\

#PhTools
SOURCES += \
	../../libs/PhTools/PhString.cpp \
	../../libs/PhTools/memorytool.cpp \
	../../libs/PhTools/PhColor.cpp \
	../../libs/PhTools/PhTime.cpp \
	../../libs/PhTools/PhTimeCode.cpp \
	../../libs/PhTools/PhFont.cpp \

HEADERS  += \
	../../libs/PhTools/PhString.h \
	../../libs/PhTools/memorytool.h \
	../../libs/PhTools/PhColor.h \
	../../libs/PhTools/PhTime.h \
	../../libs/PhTools/PhTimeCode.h \
	../../libs/PhTools/PhFont.h \

INCLUDEPATH += 	../../libs \
		/Library/Frameworks/ \


LIBS += -framework SDL -framework SDL_image -framework SDL_ttf


macx {
	copyfiles.commands = cp -r ../../data/ $${DESTDIR}/$${TARGET}.app/Contents/Resources/
}
QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles
