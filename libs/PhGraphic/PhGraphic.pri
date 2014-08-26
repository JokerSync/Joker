#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT		+= opengl

HEADERS += \
	$$PWD/PhGraphicSettings.h \
	$$PWD/PhGraphicView.h \
	$$PWD/PhQmlView.h

SOURCES += \
	$$PWD/PhGraphicView.cpp \
	$$PWD/PhQmlView.cpp

# Ubuntu specific
linux {
	INCLUDEPATH +=  /usr/include/GL
	DEPENDPATH +=   /usr/include/GL
	INCLUDEPATH += /usr/include
	LIBS += -L/usr/lib
}
