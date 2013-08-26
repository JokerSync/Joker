#ifndef PHCLOCK_H
#define PHCLOCK_H

#include <QObject>
#include "PhTimeCode.h"
#include "PhTime.h"

using namespace std;

/**
 * The PhClock class modelize a clock with its current time and rate value.
 * It can be synchronized through an external signal.
 * It fires a signal when its time and rate value changes.
 */
class PhClock : public QObject
{
	Q_OBJECT

	// Properties
	Q_PROPERTY(qint64 time READ time WRITE setTime NOTIFY timeChanged);
	Q_PROPERTY(int timeScale READ timeScale WRITE setTimeScale);
	Q_PROPERTY(float rate READ rate WRITE setRate NOTIFY rateChanged);

public:
	explicit PhClock(QObject *parent = 0);

	void setFrame(int frame, PhTimeCodeType tcType);
	int frame(PhTimeCodeType tcType) const;

	qint64 time() const	{ return _time; }

	int timeScale() const { return _timeScale; }

	float rate() const { return _rate; }

	void setTime(qint64 time);

	void setTimeScale(int timeScale);

	void setRate(float rate);

signals:
	void frameChanged();

	void timeChanged(qint64 arg);

	void rateChanged(float arg);

public slots:

	/**
	 * Sync the clock to a signal at a given frequence.
	 * The clock time value is then updated accordingly to the clock rate.
	 * @param frequence Frequence of the signal
	 */
	void tick(int frequence);

private:
	qint64 _time;

	int _timeScale;

	float _rate;

};

#endif // PHCLOCK_H
