#ifndef SONYSTRIPSYNCHRONIZER_H
#define SONYSTRIPSYNCHRONIZER_H

#include <QObject>

#include "PhTools/PhClock.h"

class SonyStripSynchronizer : public QObject
{
	Q_OBJECT
public:
	SonyStripSynchronizer();

	void setSonyClock(PhClock *clock);
	void setStripClock(PhClock *clock);

private slots:
	void onSonyFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onSonyRateChanged(PhRate rate);
	void onStripFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onStripRateChanged(PhRate rate);
private:
	PhClock * _sonyClock;
	PhClock * _stripClock;
	bool _settingStripFrame;
	bool _settingSonyFrame;
	bool _settingStripRate;
	bool _settingSonyRate;

};

#endif // SONYSTRIPSYNCHRONIZER_H
