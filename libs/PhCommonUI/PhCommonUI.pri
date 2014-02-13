#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

HEADERS += \
#    ../../libs/PhCommonUI/PhLeap.h \
	../../libs/PhCommonUI/PhMediaPanel.h \
	../../libs/PhCommonUI/PhTimecodeEdit.h \
    ../../libs/PhCommonUI/PhTimeCodeDialog.h \
    ../../libs/PhCommonUI/PhFloatingMediaPanel.h

SOURCES += \
#   ../../libs/PhCommonUI/PhLeap.cpp \
	../../libs/PhCommonUI/PhMediaPanel.cpp \
	../../libs/PhCommonUI/PhTimecodeEdit.cpp \
    ../../libs/PhCommonUI/PhTimeCodeDialog.cpp \
    ../../libs/PhCommonUI/PhFloatingMediaPanel.cpp


FORMS += ../../libs/PhCommonUI/PhMediaPanel.ui \
	../../libs/PhCommonUI/PhTimeCodeDialog.ui

#INCLUDEPATH += $$(LEAP_SDK)/include

# Windows specific
win32 {
}

# Ubuntu specific
linux {
#LIBS += -L$$(LEAP_SDK)/lib/x64 -lLeap
}


# MacOS specific
mac {
#LIBS += -L$$(LEAP_SDK)/lib -lLeap
}

#	leap.commands += cp -r $$(LEAP_SDK)/lib/libLeap.dylib $${DESTDIR}/$${TARGET}.app/Contents/Resources/libs/ ;
