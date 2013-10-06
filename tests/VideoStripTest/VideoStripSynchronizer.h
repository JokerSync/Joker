#ifndef VIDEOSTRIPSYNCHRONIZER_H
#define VIDEOSTRIPSYNCHRONIZER_H

#include <QObject>

#include "PhTools/PhClock.h"

class SonyVideoStripSynchronizer : public QObject
{
	Q_OBJECT
public:
	SonyVideoStripSynchronizer();

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

#endif // VIDEOSTRIPSYNCHRONIZER_H
