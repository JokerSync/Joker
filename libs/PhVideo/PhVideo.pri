#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

HEADERS += \
	$$PWD/PhVideoEngine.h \
	$$PWD/PhVideoSettings.h \
	$$PWD/PhVideoDecoder.h \
    $$PWD/PhVideoBuffer.h \
    $$PWD/PhVideoPool.h
SOURCES += \
	$$PWD/PhVideoEngine.cpp \
	$$PWD/PhVideoDecoder.cpp \
    $$PWD/PhVideoBuffer.cpp \
    $$PWD/PhVideoPool.cpp

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


	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_SHARED_PATH)/bin/*.dll) $${RESOURCES_PATH} $${CS}
}

# Unix specific
unix {
	FFMPEG_DEV_PATH = $$(FFMPEG_DEV_PATH)

	!exists($$FFMPEG_DEV_PATH) {
		FFMPEG_DEV_PATH = /usr/local
	}
	message("FFmpeg path: $$FFMPEG_DEV_PATH")

	INCLUDEPATH += $$FFMPEG_DEV_PATH/include
	LIBS += -L$$FFMPEG_DEV_PATH/lib

	LIBS += -lavformat -lavcodec -lavutil -lswscale
}

# MacOS specific
mac {
	LIBS += -framework CoreFoundation -framework CoreMedia -framework CoreVideo -framework VideoDecodeAcceleration -framework VideoToolbox
	LIBS += -llzma -lswresample
	LIBS += -liconv -lbz2 -lz
}
