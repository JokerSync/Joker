#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT		+= xml sql network

PRECOMPILED_HEADERS += \
    $$TOP_ROOT/libs/PhTools/PhGeneric.h \
    $$TOP_ROOT/libs/PhTools/PhFile.h \
    $$TOP_ROOT/libs/PhTools/PhData.h \

HEADERS += \
    $$TOP_ROOT/libs/PhTools/PhGeneric.h \
    $$TOP_ROOT/libs/PhTools/PhFile.h \
    $$TOP_ROOT/libs/PhTools/PhData.h \
	$$TOP_ROOT/libs/PhTools/PhDebug.h \
	$$TOP_ROOT/libs/PhTools/PhTickCounter.h \
	$$TOP_ROOT/libs/PhTools/PhPictureTools.h \
	$$TOP_ROOT/libs/PhTools/PhFileTool.h \
	$$TOP_ROOT/libs/PhTools/PhGenericSettings.h \
	$$TOP_ROOT/libs/PhTools/PhTestTools.h \

SOURCES += \
	$$TOP_ROOT/libs/PhTools/PhDebug.cpp \
	$$TOP_ROOT/libs/PhTools/PhTickCounter.cpp \
	$$TOP_ROOT/libs/PhTools/PhPictureTools.cpp \
	$$TOP_ROOT/libs/PhTools/PhFileTool.cpp \
	$$TOP_ROOT/libs/PhTools/PhGenericSettings.cpp \
	$$TOP_ROOT/libs/PhTools/PhTestTools.cpp \
