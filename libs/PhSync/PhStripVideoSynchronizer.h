#ifndef PHSTRIPVIDEOSYNCHRONIZER_H
#define PHSTRIPVIDEOSYNCHRONIZER_H

#include <QObject>

#include "PhTools/PhClock.h"

class PhStripVideoSynchronizer : public QObject
{
	Q_OBJECT
public:
	PhStripVideoSynchronizer();

	void setStripClock(PhClock *clock);
	void setVideoClock(PhClock *clock);

private slots:
	void onStripFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onStripRateChanged(PhRate rate);
	void onVideoFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onVideoRateChanged(PhRate rate);
private:
	PhClock * _stripClock;
	PhClock * _videoClock;
};

#endif // PHSTRIPVIDEOSYNCHRONIZER_H
