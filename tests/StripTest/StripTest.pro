#-------------------------------------------------
#
# Project created by QtCreator 2013-05-23T14:09:50
#
#-------------------------------------------------

TARGET = StripTest
TEMPLATE = app


QT       += core gui
QT		+= xml
QT		+= opengl

DESTDIR = ./



#Main app
SOURCES += main.cpp \
    ../../libs/PhTools/PhFileTools.cpp \
	MainController.cpp \
	../../libs/PhCommonUI/PhLeap.cpp \
    ../../libs/PhTools/PhTime.cpp \
    ../../libs/PhGraphicStrip/PhGraphicStripController.cpp \
    ../../libs/PhGraphicStrip/PhGraphicStripView.cpp \
    MainView.cpp \
    ../../libs/PhGraphic/PhGraphicController.cpp

HEADERS += \
    ../../libs/PhTools/PhFileTools.h \
	MainController.h \
	../../libs/PhCommonUI/PhLeap.h \
    ../../libs/PhGraphicStrip/PhGraphicStripController.h \
    ../../libs/PhGraphicStrip/PhGraphicStripView.h \
    MainView.h \
    ../../libs/PhGraphic/PhGraphicController.h


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


HEADERS  += \
	../../libs/PhGraphic/PhGraphicView.h \
	../../libs/PhGraphic/PhGraphicText.h \
	../../libs/PhGraphic/PhGraphicObject.h \
	../../libs/PhGraphic/PhGraphicImage.h \
	../../libs/PhGraphic/PhGraphicRect.h \
	../../libs/PhGraphic/PhGraphicTexturedRect.h \
	../../libs/PhGraphic/PhGraphicContext.h \


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
    #$$(LEAP_SDK)/include

unix {
INCLUDEPATH += /usr/include/GL \
               /usr/include/SDL \

INCLUDEPATH += ../../libs
LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
#LIBS += -L$$(LEAP_SDK)/lib/x64 -lLeap
}

macx {
LIBS += -framework SDL -framework SDL_image -framework SDL_ttf
LIBS += -L$$(LEAP_SDK)/lib -lLeap

	copyresources.commands = cp -r $${PWD}/../../data/ $${DESTDIR}/$${TARGET}.app/Contents/Resources/;
}
#QMAKE_EXTRA_TARGETS += copyresources
#POST_TARGETDEPS += copyresources
macx {

	copylibs.commands = mkdir $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs;
	copylibs.commands += cp -r /Library/Frameworks/SDL* $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs/;
}
#QMAKE_EXTRA_TARGETS += copylibs
#POST_TARGETDEPS += copylibs

macx {
	leap.commands += cp -r $$(LEAP_SDK)/lib/libLeap.dylib $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs/ ;
}
#QMAKE_EXTRA_TARGETS += leap
#POST_TARGETDEPS += leap


