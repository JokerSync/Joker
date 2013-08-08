#include "PhClock.h"

PhClock::PhClock(QObject *parent) :
	QObject(parent)
{
	//Model Init
	_rate = 0;
	_timecode = 0;

	//Timer Init
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(increaseValueTimecode()));
	_timer->start(40);
}

/****************************Getters****************************/

float PhClock::get_rate() const
{
	return _rate;
}


PhFrame PhClock::get_timecode() const
{
	return _timecode;
}


QTimer *PhClock::get_timer() const
{
	return _timer;
}
