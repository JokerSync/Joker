/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHLTCREADER_H
#define PHLTCREADER_H

#include <QString>
#include <QObject>
#include <QTime>

#include <ltc.h>
#include <portaudio.h>

#include "PhTools/PhClock.h"
#include "PhTools/PhTimeCode.h"

#define FRAME_PER_BUFFER 256
#define SAMPLE_RATE 48000

class PhLtcReader : public QObject
{

    Q_OBJECT

public:
	explicit PhLtcReader(PhTimeCodeType tcType = PhTimeCodeType25, QObject *parent = 0);

	bool init(QString deviceName="");
	void close();

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

	PaStream *stream;
	float data;

    ltc_off_t _position;
    LTCDecoder * _decoder;
    QTime _pauseDetector;

};

#endif // PHLTCREADER_H
