#-------------------------------------------------
#
# Project created by QtCreator 2014-09-01T17:54:58
#
#-------------------------------------------------

include($$TOP_ROOT/libs/PhCommonUI/PhCommonUI.pri)

HEADERS += \
	$$TOP_ROOT/specs/CommonUISpec/WindowSpecWindow.h \

SOURCES += \
	$$TOP_ROOT/specs/CommonUISpec/LockableSpinBoxSpec.cpp \
	$$TOP_ROOT/specs/CommonUISpec/MediaPanelSpec.cpp \
	$$TOP_ROOT/specs/CommonUISpec/TimeCodeEditSpec.cpp \
	$$TOP_ROOT/specs/CommonUISpec/WindowSpecWindow.cpp \
	$$TOP_ROOT/specs/CommonUISpec/WindowSpec.cpp \

FORMS += \
	$$TOP_ROOT/specs/CommonUISpec/WindowSpecWindow.ui \

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${TOP_ROOT}/data/text.txt) . $${CS}
