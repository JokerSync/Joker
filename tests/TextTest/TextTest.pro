QT       += core gui
QT		 += opengl
QT		 += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TextTest
DESTDIR = ./
TEMPLATE = app


#Main app
SOURCES += \
	main.cpp \
	TextTestWindow.cpp

HEADERS  += \
	TextTestWindow.h \


#PhGraphics
SOURCES += \
	../../libs/PhGraphic/SDLMain.cpp \
    ../../libs/PhGraphic/PhGraphicView.cpp \
	../../libs/PhGraphic/PhGraphicText.cpp \
	../../libs/PhGraphic/PhGraphicObject.cpp \
	../../libs/PhGraphic/PhGraphicImage.cpp \
	../../libs/PhGraphic/PhGraphicRect.cpp \
	../../libs/PhGraphic/PhGraphicTexturedRect.cpp \
	../../libs/PhGraphic/PhGraphicContext.cpp
HEADERS  += \
	../../libs/PhGraphic/PhGraphicView.h \
	../../libs/PhGraphic/PhGraphicText.h \
	../../libs/PhGraphic/PhGraphicObject.h \
	../../libs/PhGraphic/PhGraphicImage.h \
	../../libs/PhGraphic/PhGraphicRect.h \
	../../libs/PhGraphic/PhGraphicTexturedRect.h \
	../../libs/PhGraphic/PhGraphicContext.h

#PhTools
SOURCES += \
	../../libs/PhTools/PhString.cpp \
	../../libs/PhTools/memorytool.cpp \
	../../libs/PhTools/PhColor.cpp \
	../../libs/PhTools/PhTime.cpp \
	../../libs/PhTools/PhTimeCode.cpp \
	../../libs/PhTools/PhFont.cpp
HEADERS  += \
	../../libs/PhTools/PhString.h \
	../../libs/PhTools/memorytool.h \
	../../libs/PhTools/PhColor.h \
	../../libs/PhTools/PhTime.h \
	../../libs/PhTools/PhTimeCode.h \
	../../libs/PhTools/PhFont.h



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
