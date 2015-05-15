#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

HEADERS += \
	$$TOP_ROOT/vendor/rtmidi/RtMidi.h \
	$$PWD/PhMidiObject.h \
	$$PWD/PhMidiInput.h \
	$$PWD/PhMidiOutput.h \
	$$PWD/PhMidiTimeCodeWriter.h \
	$$PWD/PhMidiTimeCodeReader.h

SOURCES += \
	$$TOP_ROOT/vendor/rtmidi/RtMidi.cpp \
	$$PWD/PhMidiObject.cpp \
	$$PWD/PhMidiInput.cpp \
	$$PWD/PhMidiOutput.cpp \
	$$PWD/PhMidiTimeCodeWriter.cpp \
	$$PWD/PhMidiTimeCodeReader.cpp

INCLUDEPATH += $$TOP_ROOT/vendor/rtmidi

mac {
DEFINES += __MACOSX_CORE__
LIBS += -framework CoreMIDI -framework CoreAudio -framework CoreFoundation
}

linux {
DEFINES += __LINUX_ALSA__
}

win32 {
DEFINES += __WINDOWS_MM__
LIBS += -lwinmm
}
