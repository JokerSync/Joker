#ifndef PHLTCWRITER_H
#define PHLTCWRITER_H

#include <QObject>

#include <QAudioOutput>

#include "ltc.h"

#include "PhTools/PhClock.h"
#include "PhTools/PhTimeCode.h"

class PhLtcWriter : public QObject
{
	Q_OBJECT
public:
	explicit PhLtcWriter(PhTimeCodeType tcType, QObject *parent = 0);

	bool init(QString output = "");
	void close();
	static QList<QString> outputList();
	PhClock *clock();

public slots :
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);


private:
    PhClock _clock;
	LTCEncoder *_encoder;
	SMPTETimecode _st;
	double _sampleRate = 48000.0;

	QAudioOutput * _output;
	QIODevice * _buffer;



};

#endif // PHLTCWRITER_H
