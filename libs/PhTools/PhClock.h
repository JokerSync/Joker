/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHCLOCK_H
#define PHCLOCK_H

#include <QObject>

#include "PhTimeCode.h"
#include "PhDebug.h"

/**
 * The PhClock class modelize a clock with its current time and rate value.
 * It can be synchronized through an external signal.
 * It fires a signal when its time and rate value changes.
 */
class PhClock : public QObject
{
	Q_OBJECT
public:
	explicit PhClock(PhTimeCodeType tcType, QObject *parent = 0);

	void setTimeCodeType(PhTimeCodeType tcType);
	PhTimeCodeType timeCodeType() { return _tcType;}

	void setTime(PhTime time);
	PhTime time() const	{ return _time; }

	void setTimeScale(PhTimeScale timeScale);
	PhTimeScale timeScale() const { return _timeScale; }

	/**
	 * @brief Set the clock rate
	 * @param rate A rate value.
	 */
	void setRate(PhRate rate);
	PhRate rate() const { return _rate; }

	void setMillisecond(PhTime ms);
	PhTime milliSecond();

	void setFrame(PhFrame frame);
	PhFrame frame() const;

	void setTimeCode(QString tc);
	QString timeCode();

signals:
	void timeChanged(PhTime time);

	void rateChanged(PhRate rate);

	void frameChanged(PhFrame frame, PhTimeCodeType tcType);

	void tcTypeChanged(PhTimeCodeType tcType);

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
