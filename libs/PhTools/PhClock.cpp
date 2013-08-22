#include "PhClock.h"

PhClock::PhClock(QObject *parent) :
	QObject(parent)
{
	//Model Init
	_rate = 0;
	_frame = 0;

}

void PhClock::setRate(float rate)
{
	if(_rate != rate)
	{
		_rate = rate;
		rateChanged();
	}
}

void PhClock::setFrame(PhFrame frame)
{
	if(_frame != frame)
	{
		_frame = frame;

	}
}


/****************************Slots****************************/

void PhClock::tick()
{
	_frame += _rate;
	frameChanged();
	if(_frame < 0)
	{
		_frame = 0;
		_rate = 0;
	}

}


/****************************Getters****************************/

float PhClock::getRate() const
{
	return _rate;
}


PhFrame PhClock::getFrame() const
{
	return _frame;
}


PhClock::~PhClock()
{

}
