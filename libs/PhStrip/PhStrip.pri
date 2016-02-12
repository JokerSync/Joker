#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT		+= xml sql

SOURCES += \
	$$PWD/PhStripDoc.cpp \
	$$PWD/PhStripObject.cpp \
	$$PWD/PhStripCut.cpp \
	$$PWD/PhStripText.cpp \
	$$PWD/PhStripLoop.cpp \
	$$PWD/PhPeople.cpp \
    $$PWD/PhStripPeopleObject.cpp \
    $$PWD/PhStripDetect.cpp \
    $$PWD/PhStripSentence.cpp

HEADERS += \
	$$PWD/PhStripDoc.h \
	$$PWD/PhStripObject.h \
	$$PWD/PhStripCut.h \
	$$PWD/PhStripText.h \
	$$PWD/PhStripLoop.h \
	$$PWD/PhPeople.h \
    $$PWD/PhStripPeopleObject.h \
    $$PWD/PhStripDetect.h \
    $$PWD/PhStripSentence.h

