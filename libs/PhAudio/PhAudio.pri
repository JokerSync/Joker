#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

unix {
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/lib -lportaudio
}

win32 {
	!exists($$(PORTAUDIO_PATH)) {
		error("You must define PORTAUDIO_PATH")
	}

	INCLUDEPATH += $$(PORTAUDIO_PATH)\include
	LIBS += -L$$(PORTAUDIO_PATH)\lib\Win32\ReleaseMinDependency -lportaudio_x86

	QMAKE_POST_LINK += $${QMAKE_COPY} $$shell_path($$(PORTAUDIO_PATH)/lib/Win32/ReleaseMinDependency/portaudio_x86.dll) $${RESOURCES_PATH} $${CS}
}

HEADERS += \
    $$PWD/PhAudio.h \
    $$PWD/PhAudioOutput.h \
    $$PWD/PhAudioInput.h

SOURCES += \
    $$PWD/PhAudio.cpp \
    $$PWD/PhAudioOutput.cpp \
    $$PWD/PhAudioInput.cpp

