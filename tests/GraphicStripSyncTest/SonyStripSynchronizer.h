#ifndef SONYSTRIPSYNCHRONIZER_H
#define SONYSTRIPSYNCHRONIZER_H

#include <QObject>

#include "PhSync/PhClock.h"

class SonyStripSynchronizer : public QObject
{
	Q_OBJECT
public:
	SonyStripSynchronizer();

	void setSonyClock(PhClock *clock);
	void setStripClock(PhClock *clock);

private slots:
	void onSonyTimeChanged(PhTime time);
	void onSonyRateChanged(PhRate rate);
	void onStripTimeChanged(PhTime time);
	void onStripRateChanged(PhRate rate);
private:
	PhClock * _sonyClock;
	PhClock * _stripClock;
	bool _settingStripTime;
	bool _settingSonyTime;
	bool _settingStripRate;
	bool _settingSonyRate;

};

#endif // SONYSTRIPSYNCHRONIZER_H
