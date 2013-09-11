HEADERS += \
	$${JOKER_ROOT}/libs/PhTools/PhTime.h \
	$${JOKER_ROOT}/libs/PhTools/PhTimeCode.h \
	$${JOKER_ROOT}/libs/PhTools/PhDebug.h \
	$${JOKER_ROOT}/libs/PhTools/PhClock.h

SOURCES += \
    $${JOKER_ROOT}/libs/PhTools/PhTimeCode.cpp \
	$${JOKER_ROOT}/libs/PhTools/PhDebug.cpp \
    $${JOKER_ROOT}/libs/PhTools/PhClock.cpp

# Windows specific
win32 {
	RESOURCES_PATH = .
}

# Ubuntu specific
linux {
	RESOURCES_PATH = .
}


# MacOS specific
mac {
	RESOURCES_PATH = $${TARGET}.app/Contents/MacOS
}

