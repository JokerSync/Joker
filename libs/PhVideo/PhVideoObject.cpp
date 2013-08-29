#include "PhVideoObject.h"

PhVideoObject::PhVideoObject() : _clock(NULL)
{
}


void PhVideoObject::setClock(PhClock *clock)
{
	_clock = clock;
}

