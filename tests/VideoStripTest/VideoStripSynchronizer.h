#ifndef VIDEOSTRIPSYNCHRONIZER_H
#define VIDEOSTRIPSYNCHRONIZER_H

#include <QObject>

#include "PhSync/PhClock.h"

class VideoStripSynchronizer : public QObject
{
	Q_OBJECT
public:
	VideoStripSynchronizer();

	void setStripClock(PhClock *clock);
	void setVideoClock(PhClock *clock);

private slots:
	void onStripTimeChanged(PhTime time);
	void onStripRateChanged(PhRate);
	void onVideoTimeChanged(PhTime time);
	void onVideoRateChanged(PhRate rate);
private:
	PhClock * _stripClock;
	PhClock * _videoClock;
};

#endif // VIDEOSTRIPSYNCHRONIZER_H
