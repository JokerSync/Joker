#-------------------------------------------------
#
# Project created by QtCreator 2013-10-01T16:48:45
#
#-------------------------------------------------

TARGET = FFmpegTest

VERSION = 1.0.1

TOP_ROOT = $${_PRO_FILE_PWD_}/../..

include($$TOP_ROOT/common/common.pri)

SOURCES += main.cpp\
        FFmpegTestWindow.cpp

HEADERS  += FFmpegTestWindow.h

FORMS    += FFmpegTestWindow.ui

unix {
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lavformat -lavcodec -lavutil -lswscale
	LIBS += -lz
}

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
}



#LIBS += -lxvidcore -lx264 -lvorbis -lvorbisenc -lvorbisfile -lvpx
#LIBS += -ltheora -ltheoradec -ltheoraenc
#LIBS += -lspeex -lspeexdsp
#LIBS += -lschroedinger-1.0 -lopus -lvo-aacenc -lopenjpeg -lmp3lame -lfaac -lcelt0 -lfdk-aac
#LIBS += -laacplus
#to test
#LIBS += -lass -ltiff -ltiffxx -lpng -ljpeg

#LIBS += -lssl -lcrypto

mac {
	LIBS += -lbz2 -liconv
}

linux {
	LIBS += -lva
}

PH_DEPLOY_LOCATION = $$(TESTS_RELEASE_PATH)
include($$TOP_ROOT/common/deploy.pri)
