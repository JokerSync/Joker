QT += testlib

HEADERS += \
	../../libs/PhTools/PhTime.h \
	../../libs/PhTools/PhTimeCode.h \
    ../../libs/PhTools/PhClock.h \
    ../../libs/PhTools/PhDebug.h \
    ../../libs/PhTools/tests/PhTimeCodeTest.h \
    ../../libs/PhTools/PhTickCounter.h \
	../../libs/PhTools/PhPictureTools.h \
	../../libs/PhTools/ltc.h \
	../../libs/PhTools/decoder.h \
	../../libs/PhTools/encoder.h \

SOURCES += \
    ../../libs/PhTools/PhTimeCode.cpp \
    ../../libs/PhTools/PhClock.cpp \
    ../../libs/PhTools/PhDebug.cpp \
    ../../libs/PhTools/tests/PhTimeCodeTest.cpp \
    ../../libs/PhTools/PhTickCounter.cpp \
	../../libs/PhTools/PhPictureTools.cpp \
	../../libs/PhTools/ltc.c \
	../../libs/PhTools/decoder.h \
	../../libs/PhTools/encoder.h \


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
	RESOURCES_PATH = $${TARGET}.app/Contents/Resources
}

