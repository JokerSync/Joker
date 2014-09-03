#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

HEADERS += \
	../../vendor/rtmidi/RtMidi.h \
	$$TOP_ROOT/libs/PhMidi/PhMidiObject.h \
	$$TOP_ROOT/libs/PhMidi/PhMidiInput.h \
	$$TOP_ROOT/libs/PhMidi/PhMidiOutput.h \
	$$TOP_ROOT/libs/PhMidi/PhMidiTimeCodeWriter.h \
	$$TOP_ROOT/libs/PhMidi/PhMidiTimeCodeReader.h

SOURCES += \
	../../vendor/rtmidi/RtMidi.cpp \
	$$TOP_ROOT/libs/PhMidi/PhMidiObject.cpp \
	$$TOP_ROOT/libs/PhMidi/PhMidiInput.cpp \
	$$TOP_ROOT/libs/PhMidi/PhMidiOutput.cpp \
	$$TOP_ROOT/libs/PhMidi/PhMidiTimeCodeWriter.cpp \
	$$TOP_ROOT/libs/PhMidi/PhMidiTimeCodeReader.cpp

INCLUDEPATH += ../../vendor/rtmidi

mac {
DEFINES += __MACOSX_CORE__
LIBS += -framework CoreMIDI -framework CoreAudio -framework CoreFoundation
}

linux {
DEFINES += __LINUX_ALSA__
}

win {
DEFINES += __WINDOWS_MM__
}
