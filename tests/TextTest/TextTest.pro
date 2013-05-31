QT       += core gui
QT		 += opengl
QT		 += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TextTest
DESTDIR = ./
TEMPLATE = app


SOURCES += \
	main.cpp \
	TextTestWindow.cpp \
#PhGraphics
	../../libs/PhGraphic/SDLMain.cpp \
    ../../libs/PhGraphic/PhGraphicView.cpp \
	../../libs/PhGraphic/PhGraphicText.cpp \
	../../libs/PhGraphic/PhGraphicObject.cpp \
	../../libs/PhGraphic/PhGraphicImage.cpp \
	../../libs/PhGraphic/PhGraphicRect.cpp \
	../../libs/PhGraphic/PhGraphicTexturedRect.cpp \
	../../libs/PhGraphic/PhGraphicContext.cpp \
#PhTools
	../../libs/PhTools/PhString.cpp \
	../../libs/PhTools/memorytool.cpp \
	../../libs/PhTools/PhColor.cpp \
	../../libs/PhTools/PhTime.cpp \
	../../libs/PhTools/PhTimeCode.cpp \
	../../libs/PhTools/PhFont.cpp \
#PhStrip
#	../../libs/PhStrip/PhStripDoc.cpp \
#	../../libs/PhStrip/PhStripObject.cpp \
#	../../libs/PhStrip/PhStripCut.cpp \
#	../../libs/PhStrip/PhStripText.cpp \
#	../../libs/PhStrip/PhStripLoop.cpp \
#	../../libs/PhStrip/PhPeople.cpp \


HEADERS  += \
	TextTestWindow.h \
#PhGraphics
	../../libs/PhGraphic/PhGraphicView.h \
	../../libs/PhGraphic/PhGraphicText.h \
	../../libs/PhGraphic/PhGraphicObject.h \
	../../libs/PhGraphic/PhGraphicImage.h \
	../../libs/PhGraphic/PhGraphicRect.h \
	../../libs/PhGraphic/PhGraphicTexturedRect.h \
	../../libs/PhGraphic/PhGraphicContext.h \
#PhTools
	../../libs/PhTools/PhString.h \
	../../libs/PhTools/memorytool.h \
	../../libs/PhTools/PhColor.h \
	../../libs/PhTools/PhTime.h \
	../../libs/PhTools/PhTimeCode.h \
	../../libs/PhTools/PhFont.h \
#PhStrip
#	../../libs/PhStrip/PhStripDoc.h \
#	../../libs/PhStrip/PhStripObject.h \
#	../../libs/PhStrip/PhStripCut.h \
#	../../libs/PhStrip/PhStripText.h \
#	../../libs/PhStrip/PhStripLoop.h \
#	../../libs/PhStrip/PhPeople.h \


FORMS    += mainwindow.ui

LIBS += -framework SDL -framework SDL_image -framework SDL_ttf

INCLUDEPATH += /Library/Frameworks/ \
			../../libs \

DEPENDPATH += /Library/Frameworks


macx {
	copyfiles.commands = cp -r ../../data/ $${DESTDIR}/$${TARGET}.app/Contents/Resources/
}
QMAKE_EXTRA_TARGETS += copyfiles
POST_TARGETDEPS += copyfiles
