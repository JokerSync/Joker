#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT		+= xml

SOURCES += \
    ../../libs/PhStrip/PhStripDoc.cpp \
	../../libs/PhStrip/PhStripObject.cpp \
	../../libs/PhStrip/PhStripCut.cpp \
	../../libs/PhStrip/PhStripText.cpp \
	../../libs/PhStrip/PhStripLoop.cpp \
	../../libs/PhStrip/PhPeople.cpp \
    ../../libs/PhStrip/PhStripPeopleObject.cpp \
    ../../libs/PhStrip/PhStripOff.cpp \
    ../../libs/PhStrip/tests/PhStripDocTest.cpp

HEADERS += \
	../../libs/PhStrip/PhStripDoc.h \
	../../libs/PhStrip/PhStripObject.h \
	../../libs/PhStrip/PhStripCut.h \
	../../libs/PhStrip/PhStripText.h \
	../../libs/PhStrip/PhStripLoop.h \
	../../libs/PhStrip/PhPeople.h \
    ../../libs/PhStrip/PhStripPeopleObject.h \
    ../../libs/PhStrip/PhStripOff.h \
    ../../libs/PhStrip/tests/PhStripDocTest.h

# Windows specific
win32{
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/strip/test01.detx) $${RESOURCES_PATH} $${CS}

}

# Ubuntu specific
linux{
}

# MacOS specific
mac{
	QMAKE_POST_LINK += cp $${_PRO_FILE_PWD_}/../../data/strip/test01.detx .;
}

