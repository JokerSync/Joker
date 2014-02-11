#ifndef PHLTCREADER_H
#define PHLTCREADER_H

#include <QString>
#include <QObject>
#include <QTime>

#include <ltc.h>

#include "PhTools/PhClock.h"
#include "PhTools/PhTimeCode.h"

#include "PhAudio/PhAudioReader.h"

#define FRAME_PER_BUFFER 256
#define SAMPLE_RATE 48000

class PhLtcReader : public PhAudioReader
{

    Q_OBJECT

public:
	explicit PhLtcReader(PhTimeCodeType tcType = PhTimeCodeType25, QObject *parent = 0);

	bool init(QString deviceName="");

    static QList<QString> inputList();
    PhClock * clock();

private:

	int processAudio(const void *inputBuffer,
							unsigned long framesPerBuffer);
	static int audioCallback( const void *inputBuffer, void *outputBuffer,
							   unsigned long framesPerBuffer,
							   const PaStreamCallbackTimeInfo* timeInfo,
							   PaStreamCallbackFlags statusFlags,
							   void *userData );

    PhClock _clock;

	float data;

    ltc_off_t _position;
    LTCDecoder * _decoder;
    QTime _pauseDetector;

};

#endif // PHLTCREADER_H
