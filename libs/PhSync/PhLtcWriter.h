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

private:
    PhClock _clock;
	LTCEncoder *_encoder;
};

#endif // PHLTCWRITER_H
