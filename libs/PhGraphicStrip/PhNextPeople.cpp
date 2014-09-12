#include "PhGraphicStrip/PhNextPeople.h"

PhNextPeople::PhNextPeople(QString name, QString color, PhTime timeIn, bool selected)
 : PhPeople(name, color),
   _timeIn(timeIn),
   _selected(selected)
{
}

PhTime PhNextPeople::timeIn()
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

bool PhNextPeople::selected()
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
