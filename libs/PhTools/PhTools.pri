#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT += testlib

HEADERS += \
	../../libs/PhTools/PhTime.h \
	../../libs/PhTools/PhTimeCode.h \
    ../../libs/PhTools/PhClock.h \
    ../../libs/PhTools/PhDebug.h \
    ../../libs/PhTools/tests/PhTimeCodeTest.h \
    ../../libs/PhTools/PhTickCounter.h \
    ../../libs/PhTools/PhPictureTools.h \
    ../../libs/PhTools/PhFileTool.h

SOURCES += \
    ../../libs/PhTools/PhTimeCode.cpp \
    ../../libs/PhTools/PhClock.cpp \
    ../../libs/PhTools/PhDebug.cpp \
    ../../libs/PhTools/tests/PhTimeCodeTest.cpp \
    ../../libs/PhTools/PhTickCounter.cpp \
    ../../libs/PhTools/PhPictureTools.cpp \
    ../../libs/PhTools/PhFileTool.cpp

# Windows specific
win32 {
	CS = &
	CONFIG(release, debug|release) {
		RESOURCES_PATH = $$shell_path(./release/)
		message(Release mode)
	}
	CONFIG(debug, debug|release) {
		RESOURCES_PATH = $$shell_path(./debug/)
		message(Debug mode)
	}
}

# Ubuntu specific
linux {
	CS = ;
	RESOURCES_PATH = .
}


# MacOS specific
mac {
	CS = ;
	RESOURCES_PATH = $${TARGET}.app/Contents/Resources
}

DEFINES += APP_NAME=\\\"$$TARGET\\\"
