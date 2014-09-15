/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHCLOCK_H
#define PHCLOCK_H

#include <QObject>

#include "PhTimeCode.h"

/**
 * @brief The PhClock class modelize a clock with its current time and rate value.
 *
 * It can be synchronized through an external signal.
 * It emit a signal when its time and rate value changes.
 */
class PhClock : public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief The PhClock constructor
	 */
	explicit PhClock();
	/**
	 * @brief Set the clock time
	 * @param time the desired PhTime
	 */
	void setTime(PhTime time);
	/**
	 * @brief Set the clock rate
	 * @param rate the desired rate value.
	 */
	void setRate(PhRate rate);
	/**
	 * @brief Set millisecond
	 * It sets the clock PhTime using the following convertion : \f${\large \frac{timeScale * ms}{1000} }\f$
	 * @param ms the desired milliseconds value
	 */
	void setMillisecond(PhTime ms);
	/**
	 * @brief Set the clock time with a frame
	 * @param frame A frame value.
	 * @param tcType The timecode type the frame is express into.
	 */
	void setFrame(PhFrame frame, PhTimeCodeType tcType);
	/**
	 * @brief Set the clock time with a string
	 * @param tc A string containing a timecode value (HH:MM:SS:FF).
	 * @param tcType The timecode type the string is express into.
	 */
	void setTimeCode(QString tc, PhTimeCodeType tcType);
	/**
	 * @brief Get the time
	 * @return The PhTime of the clock
	 */
	PhTime time() const {
		return _time;
	}
	/**
	 * @brief Get the clock rate
	 * @return The clock PhRate
	 */
	PhRate rate() const {
		return _rate;
	}
	/**
	 * @brief Get the milliseconds of the clock
	 * @return \f${\large \frac{time * 1000}{timeScale}}\f$
	 */
	PhTime milliSecond();
	/**
	 * @brief Get the frame of the clock
	 * @param tcType A timecode type.
	 * @return A frame expressed in the desired timecode type.
	 */
	PhFrame frame(PhTimeCodeType tcType) const;
	/**
	 * @brief Get the timecode of the clock
	 * @param tcType A timecode type.
	 * @return A timecode string expressed in the desired timecode type.
	 */
	QString timeCode(PhTimeCodeType tcType);

signals:
	/**
	 * @brief emit a signal when the time changed
	 * @param time the new time
	 */
	void timeChanged(PhTime time);
	/**
	 * @brief emit a signal when the rate changed
	 * @param rate the new rate
	 */
	void rateChanged(PhRate rate);

public slots:
	/**
	 * Sync the clock to a signal at a given frequence.
	 * The clock time value is then updated accordingly to the clock rate.
	 * @param frequence Frequence of the signal
	 */
	void tick(PhTimeScale frequence);

	/**
	 * Sync the clock to a signal at a given elapse time.
	 * The clock time value is then updated accordingly to the clock rate.
	 * @param time The elapsed time value.
	 */
	void elapse(PhTime time);

private:
	PhTime _time;
	PhRate _rate;
};

#endif // PHCLOCK_H
