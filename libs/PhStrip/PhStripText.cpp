/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStripText.h"



PhStripText::PhStripText(PhTime timeIn, PhPeople *people, PhTime timeOut, float track, QString content, float height, bool selected) :
	PhStripPeopleObject(timeIn, people, timeOut, track, height),
	_content(content),
	_selected(selected)
{
}

QString PhStripText::content() const
{
	return _content;
}

bool PhStripText::selected() const
{
	return _selected;
}

void PhStripText::setSelected(bool selected)
{
	if (selected != _selected) {
		_selected = selected;
		emit selectedChanged();
	}
}
