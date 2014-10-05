#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT += widgets network

HEADERS += \
#    $$TOP_ROOT/libs/PhCommonUI/PhLeap.h \
	$$TOP_ROOT/libs/PhCommonUI/PhMediaPanel.h \
	$$TOP_ROOT/libs/PhCommonUI/PhTimeCodeEdit.h \
    $$TOP_ROOT/libs/PhCommonUI/PhTimeCodeDialog.h \
    $$TOP_ROOT/libs/PhCommonUI/PhFloatingMediaPanel.h \
    $$TOP_ROOT/libs/PhCommonUI/PhWindow.h \
    $$TOP_ROOT/libs/PhCommonUI/PhWindowSettings.h \
    $$TOP_ROOT/libs/PhCommonUI/PhDocumentWindow.h \
    $$TOP_ROOT/libs/PhCommonUI/PhDocumentWindowSettings.h \
    $$TOP_ROOT/libs/PhCommonUI/PhEditableDocumentWindow.h \
    $$TOP_ROOT/libs/PhCommonUI/PhLockableSpinBox.h \
	$$TOP_ROOT/libs/PhCommonUI/PhFeedbackDialog.h \
	$$TOP_ROOT/libs/PhCommonUI/PhFeedbackSettings.h \
    $$TOP_ROOT/libs/PhCommonUI/PhDialogButtonBox.h

SOURCES += \
#   $$TOP_ROOT/libs/PhCommonUI/PhLeap.cpp \
	$$TOP_ROOT/libs/PhCommonUI/PhMediaPanel.cpp \
	$$TOP_ROOT/libs/PhCommonUI/PhTimeCodeEdit.cpp \
    $$TOP_ROOT/libs/PhCommonUI/PhTimeCodeDialog.cpp \
    $$TOP_ROOT/libs/PhCommonUI/PhFloatingMediaPanel.cpp \
    $$TOP_ROOT/libs/PhCommonUI/PhWindow.cpp \
    $$TOP_ROOT/libs/PhCommonUI/PhDocumentWindow.cpp \
    $$TOP_ROOT/libs/PhCommonUI/PhEditableDocumentWindow.cpp \
    $$TOP_ROOT/libs/PhCommonUI/PhLockableSpinBox.cpp \
	$$TOP_ROOT/libs/PhCommonUI/PhFeedbackDialog.cpp \
    $$TOP_ROOT/libs/PhCommonUI/PhDialogButtonBox.cpp


FORMS += $$TOP_ROOT/libs/PhCommonUI/PhMediaPanel.ui \
	$$TOP_ROOT/libs/PhCommonUI/PhTimeCodeDialog.ui \
	$$TOP_ROOT/libs/PhCommonUI/PhFeedbackDialog.ui

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
