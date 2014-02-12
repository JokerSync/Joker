#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

HEADERS += \
	../../libs/PhVideo/PhVideoView.h \
    ../../libs/PhVideo/PhVideoEngine.h
SOURCES += \
    ../../libs/PhVideo/PhVideoView.cpp \
    ../../libs/PhVideo/PhVideoEngine.cpp

# Windows specific
win32{
}

# Ubuntu specific
linux {
}

# MacOS specific
mac {
	QT += widgets
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lavformat -lavcodec -lavutil -lswscale -liconv -lbz2 -lz
}
