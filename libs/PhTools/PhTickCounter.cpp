#include "PhTickCounter.h"
#include "PhDebug.h"

PhTickCounter::PhTickCounter() : _tickCount(0), _frequency(0)
{
	_timer.start();
}

void PhTickCounter::tick()
{
	_tickCount++;
	int elapsed = _timer.elapsed();
	if(elapsed > 1000)
	{
		_frequency = _tickCount;
		_timer.restart();
		_tickCount = 0;
	}
}
