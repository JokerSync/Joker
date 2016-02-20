#-------------------------------------------------
#
# Project created by QtCreator 2013-10-01T16:48:45
#
#-------------------------------------------------

TARGET = FFmpegTest

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

SOURCES += main.cpp\
        FFmpegTestWindow.cpp

HEADERS  += FFmpegTestWindow.h

FORMS    += FFmpegTestWindow.ui

mac {
	LIBS += -framework CoreFoundation -framework CoreMedia -framework CoreVideo -framework VideoDecodeAcceleration -framework VideoToolbox
	LIBS += -lbz2 -liconv -llzma -lswresample
}


unix {
	FFMPEG_DEV_PATH = $$(FFMPEG_DEV_PATH)

	!exists($$FFMPEG_DEV_PATH) {
		FFMPEG_DEV_PATH = /usr/local
	}
	message("FFmpeg path: $$FFMPEG_DEV_PATH")

	INCLUDEPATH += $$FFMPEG_DEV_PATH/include
	LIBS += -L$$FFMPEG_DEV_PATH/lib
	LIBS += -lavformat -lavcodec -lavutil -lswscale -lz
}

# Windows specific
win32 {
	!exists($$(FFMPEG_DEV_PATH)\README.txt) {
		error("You must define a valid FFMPEG_DEV_PATH")
	}

	!exists($$(FFMPEG_SHARED_PATH)\README.txt) {
		error("You must define a valid FFMPEG_SHARED_PATH")
	}

	INCLUDEPATH += $$(FFMPEG_DEV_PATH)\include
	LIBS += -L$$(FFMPEG_DEV_PATH)\lib -lavformat -lavcodec -lavutil -lswscale -liconv -lz
}

linux {
	LIBS += -lva
}

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
