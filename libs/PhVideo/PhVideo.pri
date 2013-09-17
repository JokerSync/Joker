#CONFIG += use_qtvideo
CONFIG += use_vlc

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
	QMAKE_POST_LINK += && cp -r $${VLC_PATH}/lib/* ./$${TARGET}.app/Contents/MacOS/lib
	QMAKE_POST_LINK += && cp -r $${VLC_PATH}/plugins/* ./$${TARGET}.app/Contents/MacOS/plugins
}




