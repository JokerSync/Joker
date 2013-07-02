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
	StripWindow.cpp \
	MainWindow.cpp \
    ../../libs/PhTools/PhFileTools.cpp \
	../../libs/PhCommonUI/SampleListener.cpp \
    MainController.cpp

HEADERS += StripWindow.h \
	MainWindow.h\
    ../../libs/PhTools/PhFileTools.h \
	../../libs/PhCommonUI/SampleListener.h \
	../../libs/PhCommonUI/LeapMath.h \
	../../libs/PhCommonUI/Leap.h \
    MainController.h


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

LIBS += -L../../libs/PhCommonUI/ -lLeap
LIBS += -framework SDL -framework SDL_image -framework SDL_ttf

macx {
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
	leap.commands += cp -r $${PWD}/../../libs/PhCommonUI/libLeap.dylib $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs/ ;
}
QMAKE_EXTRA_TARGETS += leap
POST_TARGETDEPS += leap


