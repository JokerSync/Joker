#include "PhTickCounter.h"
#include "PhDebug.h"

PhTickCounter::PhTickCounter()
{
	_timer.start();
}

void PhTickCounter::tick()
{
	int elapsed = _timer.elapsed();
	if(elapsed)
		_frequency = 1000.0f / elapsed;
	_timer.restart();
}
