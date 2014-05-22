#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

HEADERS += \
    ../../libs/PhVideo/PhVideoEngine.h \
    ../../libs/PhVideo/PhVideoSettings.h
SOURCES += \
    ../../libs/PhVideo/PhVideoEngine.cpp

# Windows specific
win32{
	!exists($$(FFMPEG_DEV_PATH)\README.txt) {
		error("You must define a valid FFMPEG_DEV_PATH")
	}

	!exists($$(FFMPEG_SHARED_PATH)\README.txt) {
		error("You must define a valid FFMPEG_SHARED_PATH")
	}

	INCLUDEPATH += $$(FFMPEG_DEV_PATH)\include
	LIBS += -L$$(FFMPEG_DEV_PATH)\lib -lavformat -lavcodec -lavutil -lswscale -liconv -lz


	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/swscale-2.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/avcodec-55.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/avdevice-55.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/avfilter-4.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/avformat-55.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/avutil-52.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/postproc-52.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/swresample-0.dll) $${RESOURCES_PATH} $${CS}

}

# Unix specific
unix {
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lavformat -lavcodec -lavutil -lswscale
}

# Ubuntu specific
linux {
	LIBS += -lva
}

# MacOS specific
mac {
	LIBS += -liconv -lbz2 -lz
}
