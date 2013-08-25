HEADERS += \
    $${JOKER_ROOT}/libs/PhTools/PhTimeCode.h \
    $${JOKER_ROOT}/libs/PhTools/PhTime.h
#    $${JOKER_ROOT}/libs/PhTools/PhFileTools.h \

SOURCES += \
    $${JOKER_ROOT}/libs/PhTools/PhTimeCode.cpp \
    $${JOKER_ROOT}/libs/PhTools/PhTime.cpp
#    $${JOKER_ROOT}/libs/PhTools/PhFileTools.cpp \


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

