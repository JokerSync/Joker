INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lltc -lm -lportaudio

HEADERS += \
    ../../libs/PhAudio/PhAudio.h \
    ../../libs/PhAudio/PhAudioWriter.h \
    ../../libs/PhAudio/PhAudioReader.h

SOURCES += \
    ../../libs/PhAudio/PhAudio.cpp \
    ../../libs/PhAudio/PhAudioWriter.cpp \
    ../../libs/PhAudio/PhAudioReader.cpp

