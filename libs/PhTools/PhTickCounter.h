/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef TICKCOUNTER_H
#define TICKCOUNTER_H

#include "PhTools/PhGeneric.h"

/**
 * @brief Utility that allow to meter the frequency of a signal
 * connected to the tick slot
 *
 * It takes 1 second to be accurate.
 */
class PhTickCounter
{
public:
	PhTickCounter();

	/**
	 * @brief Get frequency
	 * @return the measured frequency
	 */
	int frequency() {
		return _frequency;
	}

public slots:
	/**
	 * @brief Handle the tick
	 *
	 * This slot must be called every time the measured action occurs
	 */
	void tick();

private:
	QTime _timer;
	int _frequency;
	int _tickCount;
};

#endif // TICKCOUNTER_H
