#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

message("Using Qt v$$QT_VERSION")

HEADERS += \
	$$PWD/PhDebug.h \
	$$PWD/PhTickCounter.h \
	$$PWD/PhPictureTools.h \
	$$PWD/PhFileTool.h \
	$$PWD/PhGenericSettings.h \

SOURCES += \
	$$PWD/PhDebug.cpp \
	$$PWD/PhTickCounter.cpp \
	$$PWD/PhPictureTools.cpp \
	$$PWD/PhFileTool.cpp \
	$$PWD/PhGenericSettings.cpp \
