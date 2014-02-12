/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef TICKCOUNTER_H
#define TICKCOUNTER_H

#include <QTime>

/**
 * TickCounter is an utility that allow to meter the frequency of a signal
 * connected to the tick slot.
 */
class PhTickCounter
{
public:
	PhTickCounter();

	int frequency() { return _frequency; }

public slots:
	void tick();

private:
	QTime _timer;
	int _frequency;
	int _tickCount;
};

#endif // TICKCOUNTER_H
