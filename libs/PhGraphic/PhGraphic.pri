#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT		+= opengl

HEADERS += \
	$$PWD/PhGraphicSettings.h \
	$$PWD/PhGraphicView.h \
	$$PWD/PhGraphicImage.h \
	$$PWD/PhGraphicText.h \
	$$PWD/PhGraphicTexturedRect.h \
	$$PWD/PhFont.h \
	$$PWD/PhGraphicObject.h \
	$$PWD/PhGraphicRect.h \
	$$PWD/PhGraphicSolidRect.h \
	$$PWD/PhGraphicDisc.h \
	$$PWD/PhGraphicDashedLine.h \
	$$PWD/PhGraphicArrow.h \

SOURCES += \
	$$PWD/PhGraphicView.cpp \
	$$PWD/PhGraphicImage.cpp \
	$$PWD/PhGraphicText.cpp \
	$$PWD/PhGraphicTexturedRect.cpp \
	$$PWD/PhFont.cpp \
	$$PWD/PhGraphicObject.cpp \
	$$PWD/PhGraphicRect.cpp \
	$$PWD/PhGraphicSolidRect.cpp \
	$$PWD/PhGraphicDisc.cpp \
	$$PWD/PhGraphicDashedLine.cpp \
	$$PWD/PhGraphicArrow.cpp

# Ubuntu specific
linux {
	INCLUDEPATH +=  /usr/include/GL
	DEPENDPATH +=   /usr/include/GL
	INCLUDEPATH += /usr/include
	LIBS += -L/usr/lib
}
