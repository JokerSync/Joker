#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

HEADERS += \
	../../libs/PhMidi/PhMidiObject.h \
	../../libs/PhMidi/PhMidiInput.h \
	../../libs/PhMidi/PhMidiOutput.h \
	../../vendor/rtmidi/RtMidi.h

SOURCES += \
	../../libs/PhMidi/PhMidiObject.cpp \
	../../libs/PhMidi/PhMidiInput.cpp \
	../../libs/PhMidi/PhMidiOutput.cpp \
../../vendor/rtmidi/RtMidi.cpp

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
