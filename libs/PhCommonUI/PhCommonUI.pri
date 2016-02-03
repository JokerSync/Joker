#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

QT += widgets network

HEADERS += \
#    $$PWD/PhLeap.h \
	$$PWD/PhMediaPanel.h \
	$$PWD/PhTimeCodeEdit.h \
    $$PWD/PhTimeCodeDialog.h \
    $$PWD/PhFloatingMediaPanel.h \
    $$PWD/PhWindow.h \
    $$PWD/PhWindowSettings.h \
    $$PWD/PhDocumentWindow.h \
    $$PWD/PhDocumentWindowSettings.h \
    $$PWD/PhEditableDocumentWindow.h \
    $$PWD/PhLockableSpinBox.h \
	$$PWD/PhFeedbackDialog.h \
	$$PWD/PhFeedbackSettings.h \
    $$PWD/PhDialogButtonBox.h \

SOURCES += \
#   $$PWD/PhLeap.cpp \
	$$PWD/PhMediaPanel.cpp \
	$$PWD/PhTimeCodeEdit.cpp \
    $$PWD/PhTimeCodeDialog.cpp \
    $$PWD/PhFloatingMediaPanel.cpp \
    $$PWD/PhWindow.cpp \
    $$PWD/PhDocumentWindow.cpp \
    $$PWD/PhEditableDocumentWindow.cpp \
    $$PWD/PhLockableSpinBox.cpp \
	$$PWD/PhFeedbackDialog.cpp \
    $$PWD/PhDialogButtonBox.cpp


FORMS += $$PWD/PhMediaPanel.ui \
	$$PWD/PhTimeCodeDialog.ui \
	$$PWD/PhFeedbackDialog.ui

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
