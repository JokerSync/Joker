#ifndef STRIPVIDEOSYNCHRONIZER_H
#define STRIPVIDEOSYNCHRONIZER_H

#include <QObject>

#include "PhTools/PhClock.h"

class StripVideoSynchronizer : public QObject
{
	Q_OBJECT
public:
	StripVideoSynchronizer();

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

#endif // STRIPVIDEOSYNCHRONIZER_H
