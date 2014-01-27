#ifndef SONYVIDEOSTRIPSYNCHRONIZER_H
#define SONYVIDEOSTRIPSYNCHRONIZER_H

#include <QObject>

#include "PhTools/PhClock.h"


class VideoStripSynchronizer : public QObject
{
	Q_OBJECT
public:
	enum SyncType {
		NoSync = 0,
		Sony = 1,
		LTC = 2,
	} ;

	VideoStripSynchronizer();

	void setStripClock(PhClock *clock);
	PhClock * stripClock() { return _stripClock; }
	void setVideoClock(PhClock *clock);
	PhClock * videoClock() { return _videoClock; }
    void setSyncClock(PhClock *clock, SyncType type);
    PhClock * syncClock() { return _syncClock; }

private slots:
	void onStripFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onStripRateChanged(PhRate rate);
	void onVideoFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onVideoRateChanged(PhRate rate);
	void onVideoTCTypeChanged(PhTimeCodeType tcType);
	void onSyncFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onSyncRateChanged(PhRate rate);
private:
	int _syncType;
	PhClock * _stripClock;
	PhClock * _videoClock;
    PhClock * _syncClock;
	bool _settingStripFrame;
	bool _settingVideoFrame;
	bool _settingSonyFrame;
	bool _settingStripRate;
	bool _settingVideoRate;
	bool _settingSonyRate;
};

#endif // SONYVIDEOSTRIPSYNCHRONIZER_H
