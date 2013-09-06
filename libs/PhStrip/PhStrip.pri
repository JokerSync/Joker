QT		+= xml

SOURCES += \
    ../../libs/PhStrip/PhStripDoc.cpp \
	../../libs/PhStrip/PhStripObject.cpp \
	../../libs/PhStrip/PhStripCut.cpp \
	../../libs/PhStrip/PhStripText.cpp \
	../../libs/PhStrip/PhStripLoop.cpp \
	../../libs/PhStrip/PhPeople.cpp \

HEADERS += \
	../../libs/PhStrip/PhStripDoc.h \
	../../libs/PhStrip/PhStripObject.h \
	../../libs/PhStrip/PhStripCut.h \
	../../libs/PhStrip/PhStripText.h \
	../../libs/PhStrip/PhStripLoop.h \
	../../libs/PhStrip/PhPeople.h \

# Windows specific
win32 {
}

# Ubuntu specific
linux {
}

# MacOS specific
mac {
}


