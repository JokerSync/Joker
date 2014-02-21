#
# Copyright (C) 2012-2014 Phonations
# License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
#

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lportaudio

HEADERS += \
    ../../libs/PhAudio/PhAudio.h \
    ../../libs/PhAudio/PhAudioWriter.h \
    ../../libs/PhAudio/PhAudioReader.h

SOURCES += \
    ../../libs/PhAudio/PhAudio.cpp \
    ../../libs/PhAudio/PhAudioWriter.cpp \
    ../../libs/PhAudio/PhAudioReader.cpp

