TEMPLATE = app
TARGET = VideoSyncTest

INCLUDEPATH += ../../libs

include(../../libs/PhTools/PhTools.pri)
include(../../libs/PhAudio/PhAudio.pri)
include(../../libs/PhSync/PhSync.pri)
include(../../libs/PhGraphic/PhGraphic.pri)
include(../../libs/PhVideo/PhVideo.pri)
include(../../libs/PhCommonUI/PhCommonUI.pri)

HEADERS += VideoSyncTestWindow.h \
	VideoSyncTestSettings.h \
	../VideoTest/VideoTestSettings.h \
	../VideoTest/VideoTestWindow.h

SOURCES += \
	main.cpp \
	VideoSyncTestWindow.cpp \
	../VideoTest/VideoTestWindow.cpp

FORMS += \
	../VideoTest/VideoTestWindow.ui

QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($${JOKER_ROOT}/data/fonts/HelveticaCYPlain.ttf) $${RESOURCES_PATH} $${CS}
