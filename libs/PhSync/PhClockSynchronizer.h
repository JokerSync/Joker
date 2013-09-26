#ifndef PHCLOCKSYNCHRONIZER_H
#define PHCLOCKSYNCHRONIZER_H

#include <QObject>

#include "PhTools/PhClock.h"

class PhClockSynchronizer : public QObject
{
	Q_OBJECT
public:
	PhClockSynchronizer();

	void setPrimaryClock(PhClock *clock);
	void setSecondaryClock(PhClock *clock);

private slots:
	void onFrameChanged1(PhFrame frame, PhTimeCodeType tcType);
	void onRateChanged1(PhRate rate);
	void onFrameChanged2(PhFrame frame, PhTimeCodeType tcType);
	void onRateChanged2(PhRate rate);
private:
	PhClock * _clock1;
	PhClock * _clock2;
};

#endif // PHCLOCKSYNCHRONIZER_H
