QT		+= xml

SOURCES += \
    $${JOKER_ROOT}/libs/PhStrip/PhStripDoc.cpp \
	$${JOKER_ROOT}/libs/PhStrip/PhStripObject.cpp \
	$${JOKER_ROOT}/libs/PhStrip/PhStripCut.cpp \
	$${JOKER_ROOT}/libs/PhStrip/PhStripText.cpp \
	$${JOKER_ROOT}/libs/PhStrip/PhStripLoop.cpp \
	$${JOKER_ROOT}/libs/PhStrip/PhPeople.cpp \

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


