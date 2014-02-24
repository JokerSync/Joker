#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lportaudio

HEADERS += \
    ../../libs/PhAudio/PhAudio.h \
    ../../libs/PhAudio/PhAudioOutput.h \
    ../../libs/PhAudio/PhAudioInput.h

SOURCES += \
    ../../libs/PhAudio/PhAudio.cpp \
    ../../libs/PhAudio/PhAudioOutput.cpp \
    ../../libs/PhAudio/PhAudioInput.cpp

