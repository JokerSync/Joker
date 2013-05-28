QT       += core gui
QT		 += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TextTest
DESTDIR = ./
TEMPLATE = app


SOURCES += \
	main.cpp\
    SDLMain.cpp \
	TextTestWindow.cpp \
#PhGraphics
    ../../libs/PhGraphic/PhGraphicView.cpp \
	../../libs/PhGraphic/PhGraphicText.cpp \
#PhTools
	../../libs/PhTools/PhString.cpp \
	../../libs/PhTools/memorytool.cpp \
	../../libs/PhTools/PhColor.cpp \
	../../libs/PhTools/PhTime.cpp \
	../../libs/PhTools/PhTimeCode.cpp \
#PhStrip
	../../libs/PhStrip/PhStripDoc.cpp \
	../../libs/PhStrip/PhStripObject.cpp \
	../../libs/PhStrip/PhStripCut.cpp \
	../../libs/PhStrip/PhStripText.cpp \
	../../libs/PhStrip/PhStripLoop.cpp \
	../../libs/PhStrip/PhPeople.cpp \


HEADERS  += \
	TextTestWindow.h \
#PhGraphics
	../../libs/PhGraphic/PhGraphicView.h \
	../../libs/PhGraphic/PhGraphicText.h \
#PhTools
	../../libs/PhTools/PhString.h \
	../../libs/PhTools/memorytool.h \
	../../libs/PhTools/PhColor.h \
	../../libs/PhTools/PhTime.h \
	../../libs/PhTools/PhTimeCode.h \
#PhStrip
	../../libs/PhStrip/PhStripDoc.h \
	../../libs/PhStrip/PhStripObject.h \
	../../libs/PhStrip/PhStripCut.h \
	../../libs/PhStrip/PhStripText.h \
	../../libs/PhStrip/PhStripLoop.h \
	../../libs/PhStrip/PhPeople.h \


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
