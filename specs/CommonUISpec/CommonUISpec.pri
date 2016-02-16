#-------------------------------------------------
#
# Project created by QtCreator 2014-09-01T17:54:58
#
#-------------------------------------------------

include($$TOP_ROOT/libs/PhCommonUI/PhCommonUI.pri)

HEADERS += \
	$$PWD/WindowSpecSettings.h \
	$$PWD/WindowSpecWindow.h \
    $$PWD/DocumentWindowSpecSettings.h \
    $$PWD/DocumentWindowSpecWindow.h

SOURCES += \
	$$PWD/LockableSpinBoxSpec.cpp \
	$$PWD/MediaPanelSpec.cpp \
	$$PWD/TimeCodeEditSpec.cpp \
	$$PWD/WindowSpecWindow.cpp \
	$$PWD/WindowSpec.cpp \
    $$PWD/DocumentWindowSpecWindow.cpp \
    $$PWD/DocumentWindowSpec.cpp

FORMS += \
	$$PWD/WindowSpecWindow.ui \
	$$PWD/DocumentWindowSpecWindow.ui
