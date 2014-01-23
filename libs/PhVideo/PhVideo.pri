HEADERS += \
	../../libs/PhVideo/PhVideoView.h \
    ../../libs/PhVideo/PhVideoEngine.h
SOURCES += \
    ../../libs/PhVideo/PhVideoView.cpp \
    ../../libs/PhVideo/PhVideoEngine.cpp

# Windows specific
win32{
	!exists($$(FFMPEG_PATH)) {
		error("You must define FFMPEG_PATH")
	}
##### YOU HAVE TO ADD THESES LINES ON COMMON.H @ L29 :
#####
#####	#ifndef INT64_C
#####	#define INT64_C(c) (c ## LL)
#####	#define UINT64_C(c) (c ## ULL)
#####	#endif
#####
##### see http://code.google.com/p/ffmpegsource/issues/detail?id=11#c13
##### and http://code.google.com/p/ffmpegsource/issues/detail?id=11#c23
#####
	INCLUDEPATH += $$(FFMPEG_PATH)\include
	LIBS += -L$$(FFMPEG_PATH)\lib -lavformat -lavcodec -lavutil -lswscale -liconv -lz


	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_PATH)/bin/swscale-2.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_PATH)/bin/avcodec-55.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_PATH)/bin/avdevice-55.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_PATH)/bin/avfilter-4.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_PATH)/bin/avformat-55.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_PATH)/bin/avutil-52.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_PATH)/bin/postproc-52.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_PATH)/bin/SDL2_ttf.dll) $${RESOURCES_PATH} $${CS}
	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(FFMPEG_PATH)/bin/swresample-0.dll) $${RESOURCES_PATH} $${CS}

}

# Ubuntu specific
linux {
}

# MacOS specific
mac {
	QT += widgets
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lavformat -lavcodec -lavutil -lswscale -liconv -lbz2 -lz
}
