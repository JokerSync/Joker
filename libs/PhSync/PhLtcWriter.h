#ifndef PHLTCWRITER_H
#define PHLTCWRITER_H

#include <QObject>


#include <ltc.h>
#include <portaudio.h>

#include "PhTools/PhClock.h"
#include "PhTools/PhTimeCode.h"

#define FRAME_PER_BUFFER 1920
#define SAMPLE_RATE 48000

class PhLtcWriter : public QObject
{
	Q_OBJECT
public:
	explicit PhLtcWriter(PhTimeCodeType tcType, QObject *parent = 0);

	bool init(QString deviceName = "");
	void close();
	static QList<QString> outputList();
	PhClock *clock();

private:

	int processAudio(void *outputBuffer,
							unsigned long framesPerBuffer);
	static int audioCallback( const void *inputBuffer, void *outputBuffer,
							   unsigned long framesPerBuffer,
							   const PaStreamCallbackTimeInfo* timeInfo,
							   PaStreamCallbackFlags statusFlags,
							   void *userData );
	PaStream *stream;
	float data;

    PhClock _clock;
	LTCEncoder *_encoder;
	SMPTETimecode _st;

};

#endif // PHLTCWRITER_H
