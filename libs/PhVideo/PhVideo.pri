#CONFIG += use_qtvideo
#CONFIG += use_vlc
#CONFIG += use_qtav
CONFIG += use_ffmpeg
#CONFIG += use_vlc

HEADERS += ../../libs/PhVideo/PhVideoObject.h
HEADERS += ../../libs/PhVideo/PhVideoView.h
SOURCES += ../../libs/PhVideo/PhVideoObject.cpp

# Windows specific
win32{
}

# Ubuntu specific
linux {
	VLC_PATH = /usr/lib/vlc
}

# MacOS specific
mac {
	QT += widgets
	VLC_PATH = /Applications/VLC.app/Contents/MacOS/
}

use_qtvideo {
	DEFINES += USE_QTVIDEO
	QT += multimedia multimediawidgets

	HEADERS += ../../libs/PhVideo/PhQTVideoView.h
	SOURCES += ../../libs/PhVideo/PhQTVideoView.cpp
}

use_vlc {
	DEFINES += USE_VLC
	INCLUDEPATH += $${VLC_PATH}/include
	DEPENDPATH += $${VLC_PATH}/include
	LIBS += -L$${VLC_PATH}/lib/ -lvlc

	HEADERS += ../../libs/PhVideo/PhVLCVideoView.h
	SOURCES += ../../libs/PhVideo/PhVLCVideoView.cpp

	# Copy vlc dynamic libraries and plugins to output:
	QMAKE_POST_LINK += mkdir -p ./$${TARGET}.app/Contents/MacOS/lib
	QMAKE_POST_LINK += && mkdir -p ./$${TARGET}.app/Contents/MacOS/plugins
	QMAKE_POST_LINK += && cp -r $${VLC_PATH}/lib/libvlc.5.dylib ./$${TARGET}.app/Contents/MacOS/lib
	QMAKE_POST_LINK += && cp -r $${VLC_PATH}/lib/libvlccore.7.dylib ./$${TARGET}.app/Contents/MacOS/lib
	QMAKE_POST_LINK += && ln -sf ./$${TARGET}.app/Contents/MacOS/lib/libvlc.5.dylib ./$${TARGET}.app/Contents/MacOS/lib/libvlc.dylib
	QMAKE_POST_LINK += && ln -sf ./$${TARGET}.app/Contents/MacOS/lib/libvlccore.7.dylib ./$${TARGET}.app/Contents/MacOS/lib/libvlccore.dylib

	QMAKE_POST_LINK += && cp -r $${VLC_PATH}/plugins/* ./$${TARGET}.app/Contents/MacOS/plugins
	QMAKE_POST_LINK += && echo "all vlc lib and plugin copy ok"
}

use_qtav {
	DEFINES += USE_QTAV
	INCLUDEPATH += /usr/local/include /usr/local/include/QtAV

	HEADERS += ../../libs/PhVideo/PhQtAVVideoView.h
	SOURCES += ../../libs/PhVideo/PhQtAVVideoView.cpp

	LIBS += -L/usr/local/lib
	LIBS += -lQtAV

	QMAKE_POST_LINK += cp /usr/local/lib/libQtA*.dylib $${TARGET}.app/Contents/MacOS;
}

use_ffmpeg {
	DEFINES += USE_FFMPEG
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lavformat -lavcodec -lavutil

	LIBS += -lxvidcore -lx264 -lvorbis -lvorbisenc -lvorbisfile -lvpx
	LIBS += -ltheora -ltheoradec -ltheoraenc
	LIBS += -lspeex -lspeexdsp
	LIBS += -lschroedinger-1.0 -lopus -lvo-aacenc -lopenjpeg -lmp3lame -lfaac -lcelt0 -lfdk-aac
	LIBS += -lswscale -laacplus
	#to test
	LIBS += -lass -ltiff -ltiffxx -lpng -ljpeg

	LIBS += -lssl -lcrypto -lbz2 -lz
	LIBS += -liconv

	HEADERS += ../../libs/PhVideo/PhFFMpegVideoView.h
	SOURCES += ../../libs/PhVideo/PhFFMpegVideoView.cpp

	LIBS += -L/usr/local/lib
	LIBS += -lQtAV

	QMAKE_POST_LINK += cp /usr/local/lib/libQtA*.dylib $${TARGET}.app/Contents/MacOS;
}
