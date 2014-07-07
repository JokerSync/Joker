#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT += widgets network


HEADERS += \
#    ../../libs/PhCommonUI/PhLeap.h \
	../../libs/PhCommonUI/PhMediaPanel.h \
	../../libs/PhCommonUI/PhTimeCodeEdit.h \
    ../../libs/PhCommonUI/PhTimeCodeDialog.h \
    ../../libs/PhCommonUI/PhFloatingMediaPanel.h \
    ../../libs/PhCommonUI/PhDocumentWindow.h \
    ../../libs/PhCommonUI/PhDocumentWindowSettings.h \
    ../../libs/PhCommonUI/PhWindow.h \
    ../../libs/PhCommonUI/PhWindowSettings.h \
    ../../libs/PhCommonUI/PhLockableSpinBox.h \
	../../libs/PhCommonUI/PhFeedbackDialog.h \
	../../libs/PhCommonUI/PhFeedbackSettings.h \
    ../../libs/PhCommonUI/PhDialogButtonBox.h

SOURCES += \
#   ../../libs/PhCommonUI/PhLeap.cpp \
	../../libs/PhCommonUI/PhMediaPanel.cpp \
	../../libs/PhCommonUI/PhTimeCodeEdit.cpp \
    ../../libs/PhCommonUI/PhTimeCodeDialog.cpp \
    ../../libs/PhCommonUI/PhFloatingMediaPanel.cpp \
    ../../libs/PhCommonUI/PhDocumentWindow.cpp \
    ../../libs/PhCommonUI/PhWindow.cpp \
    ../../libs/PhCommonUI/PhLockableSpinBox.cpp \
	../../libs/PhCommonUI/PhFeedbackDialog.cpp \
    ../../libs/PhCommonUI/PhDialogButtonBox.cpp


FORMS += ../../libs/PhCommonUI/PhMediaPanel.ui \
	../../libs/PhCommonUI/PhTimeCodeDialog.ui \
	../../libs/PhCommonUI/PhFeedbackDialog.ui

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
