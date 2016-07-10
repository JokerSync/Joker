#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT		+= xml sql network

PRECOMPILED_HEADERS += \
    $$PWD/PhGeneric.h \
    $$PWD/PhFile.h \
    $$PWD/PhData.h \

HEADERS += \
    $$PWD/PhGeneric.h \
    $$PWD/PhFile.h \
    $$PWD/PhData.h \
	$$PWD/PhDebug.h \
	$$PWD/PhTickCounter.h \
	$$PWD/PhPictureTools.h \
	$$PWD/PhFileTool.h \
	$$PWD/PhGenericSettings.h \
	$$PWD/PhTestTools.h \

SOURCES += \
	$$PWD/PhDebug.cpp \
	$$PWD/PhTickCounter.cpp \
	$$PWD/PhPictureTools.cpp \
	$$PWD/PhFileTool.cpp \
	$$PWD/PhTestTools.cpp
