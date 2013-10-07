#ifndef SONYVIDEOSTRIPSYNCHRONIZER_H
#define SONYVIDEOSTRIPSYNCHRONIZER_H

#include <QObject>

#include "PhTools/PhClock.h"

class VideoStripSynchronizer : public QObject
{
	Q_OBJECT
public:
	VideoStripSynchronizer();

	void setStripClock(PhClock *clock);
	void setVideoClock(PhClock *clock);
	void setSonyClock(PhClock *clock);

private slots:
	void onStripFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onStripRateChanged(PhRate rate);
	void onVideoFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onVideoRateChanged(PhRate rate);
	void onSonyFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onSonyRateChanged(PhRate rate);
private:
	PhClock * _stripClock;
	PhClock * _videoClock;
	PhClock * _sonyClock;
	bool _settingStripFrame;
	bool _settingVideoFrame;
	bool _settingSonyFrame;
	bool _settingStripRate;
	bool _settingVideoRate;
	bool _settingSonyRate;
};

#endif // SONYVIDEOSTRIPSYNCHRONIZER_H
