#ifndef PHCLOCK_H
#define PHCLOCK_H

#include <QObject>
#include "PhTimeCode.h"

/**
  * @brief PhTime is a type used for time value.
  */
typedef qint64 PhTime;

/**
  * @brief PhTimeScale is a type used for time scale value.
  */
typedef int PhTimeScale;

/**
  * @brief PhRate is a type used for rate value.
  */
typedef double PhRate;

/**
 * The PhClock class modelize a clock with its current time and rate value.
 * It can be synchronized through an external signal.
 * It fires a signal when its time and rate value changes.
 */
class PhClock : public QObject
{
	Q_OBJECT
public:
	explicit PhClock(QObject *parent = 0);

	void setTCType(PhTimeCodeType tcType);
	PhTimeCodeType getTCType() { return _tcType;}

	void setTime(PhTime time);
	PhTime time() const	{ return _time; }

	void setTimeScale(PhTimeScale timeScale);
	PhTimeScale timeScale() const { return _timeScale; }

	void setRate(PhRate rate);
	PhRate rate() const { return _rate; }

	void setFrame(PhFrame frame);
	int frame() const;


signals:
	void timeChanged(PhTime time);

	void rateChanged(PhRate rate);

	void frameChanged();

public slots:

	/**
	 * Sync the clock to a signal at a given frequence.
	 * The clock time value is then updated accordingly to the clock rate.
	 * @param frequence Frequence of the signal
	 */
	void tick(PhTimeScale frequence);

private:
	PhTimeCodeType _tcType;
	PhTime _time;
	PhTimeScale _timeScale;

	PhRate _rate;

};

#endif // PHCLOCK_H
