#-------------------------------------------------
#
# Project created by QtCreator 2013-07-04T22:19:13
#
#-------------------------------------------------

QT       += core gui
QT		+= xml
QT		+= opengl
QT += multimedia multimediawidgets


DESTDIR = ./

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Joker
TEMPLATE = app


SOURCES += main.cpp\
        JokerMainWindow.cpp \
        ../../tests/StripTest/StripWindow.cpp \
        ../../tests/StripTest/MainController.cpp \
        ../../libs/PhCommonUI/PhLeap.cpp
#    ../../tests/StripTest/StripWindow.cpp \
#	../../tests/StripTest/MainController.cppk


HEADERS  += JokerMainWindow.h \
        ../../tests/StripTest/StripWindow.h \
            ../../tests/StripTest/MainController.h \
        ../../libs/PhCommonUI/PhLeap.h
#    ../../tests/StripTest/StripWindow.h \
#	../../tests/StripTest/MainController.h


FORMS    += JokerMainWindow.ui


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
             ../../tests/StripTest/
                #$$(LEAP_SDK)/include \
             #   ../../tests/StripTest/


unix {
INCLUDEPATH += /usr/include/GL \
               /usr/include/SDL \

INCLUDEPATH += ../../libs
LIBS += -lSDL -lSDL_image -lSDL_mixer -lSDL_ttf
#LIBS += -L$$(LEAP_SDK)/lib/x64 -lLeap
}


macx {
LIBS += -L$$(LEAP_SDK)/lib -lLeap
LIBS += -framework SDL -framework SDL_image -framework SDL_ttf
	copyresources.commands = cp -r $${PWD}/../../data/ $${DESTDIR}/$${TARGET}.app/Contents/Resources/;
}
QMAKE_EXTRA_TARGETS += copyresources
POST_TARGETDEPS += copyresources
macx {

	copylibs.commands = mkdir $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs;
	copylibs.commands += cp -r /Library/Frameworks/SDL* $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs/;
}
QMAKE_EXTRA_TARGETS += copylibs
POST_TARGETDEPS += copylibs

macx {
	leap.commands += cp -r $$(LEAP_SDK)/lib/libLeap.dylib $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs/ ;
}
QMAKE_EXTRA_TARGETS += leap
POST_TARGETDEPS += leap
