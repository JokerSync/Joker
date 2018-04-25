#include "PhGraphicStrip/PhNextPeople.h"

PhNextPeople::PhNextPeople(QString name, QString color, PhTime timeIn, bool selected, PhTime duration)
	: PhPeople(name, color),
	_timeIn(timeIn),
	_selected(selected),
	_duration(duration)
{
}

PhTime PhNextPeople::timeIn() const
{
	return _timeIn;
}

void PhNextPeople::setTimeIn(PhTime timeIn)
{
	if (timeIn != _timeIn) {
		_timeIn = timeIn;
		emit timeInChanged();
	}
}

PhTime PhNextPeople::duration() const
{
	return _duration;
}

void PhNextPeople::setDuration(PhTime duration)
{
	if (duration != _duration) {
		_duration = duration;
		emit durationChanged();
	}
}

bool PhNextPeople::selected() const
{
	return _selected;
}

void PhNextPeople::setSelected(bool selected)
{
	if (selected != _selected) {
		_selected = selected;
		emit selectedChanged();
	}
}
