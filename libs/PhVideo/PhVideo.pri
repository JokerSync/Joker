#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

HEADERS += \
    $$PWD/PhVideoEngine.h \
	$$PWD/PhVideoSettings.h \
	$$PWD/PhVideoSurface.h
SOURCES += \
	$$PWD/PhVideoEngine.cpp \
	$$PWD/PhVideoSurface.cpp

# Windows specific
win32{
	!exists($$(FFMPEG_DEV_PATH)\README.txt) {
		error("You must define a valid FFMPEG_DEV_PATH")
	}

	!exists($$(FFMPEG_SHARED_PATH)\README.txt) {
		error("You must define a valid FFMPEG_SHARED_PATH")
	}

	INCLUDEPATH += $$(FFMPEG_DEV_PATH)\include
	LIBS += -L$$(FFMPEG_DEV_PATH)\lib -lavformat -lavcodec -lavutil -lswscale -liconv

	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/avcodec-56.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/avdevice-56.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/avfilter-5.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/avformat-56.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/avutil-54.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/postproc-53.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/swresample-1.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/swscale-3.dll) $${RESOURCES_PATH} $${CS}
}

# Unix specific
unix {
	LIBS += -lavformat -lavcodec -lavutil -lswscale
}

# Ubuntu specific
linux {
	LIBS += -lva
}

# MacOS specific
mac {
	LIBS += -liconv -lbz2 -lz
}
