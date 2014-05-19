/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHCLOCK_H
#define PHCLOCK_H

#include <QObject>

#include "PhTimeCode.h"
#include "PhDebug.h"

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
	 * @brief PhClock constructor
	 * @param tcType the desired PhTimeCodeType
	 */
	explicit PhClock(PhTimeCodeType tcType);
	/**
	 * @brief Set the timecode type
	 * @param tcType the desired PhTimeCodeType
	 */
	void setTimeCodeType(PhTimeCodeType tcType);
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
	 * @brief Set the clock frame
	 * @param frame the desired PhFrame
	 */
	void setFrame(PhFrame frame);
	/**
	 * @brief Set the timecode
	 * The conversion between the string and the time code is done undercover using PhTimeCode::frameFromString
	 * @param tc the desired timecode
	 */
	void setTimeCode(QString tc);
	/**
	 * @brief Get the timecode type
	 * @return The PhTimeCodeType of the clock
	 */
	PhTimeCodeType timeCodeType() {
		return _tcType;
	}
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
	 * @brief Get the PhFrame of the clock
	 * @return The PhFrame of the clock
	 */
	PhFrame frame() const;
	/**
	 * @brief Get the timecode of the clock
	 * @return The timecode of the clock
	 */
	QString timeCode();


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
	/**
	 * @brief emit a signal when the frame changed
	 * @param frame the new frame
	 * @param tcType the corresponding PhTimeCodeType
	 */
	void frameChanged(PhFrame frame, PhTimeCodeType tcType);
	/**
	 * @brief emit a signal when the timecode type changed
	 * @param tcType the new PhTimeCodeType
	 */
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

	PhRate _rate;

};

#endif // PHCLOCK_H
