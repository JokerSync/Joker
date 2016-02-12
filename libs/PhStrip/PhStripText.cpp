/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStripText.h"



PhStripText::PhStripText(PhTime timeIn, PhPeople *people, PhTime timeOut, float y, QString content, float height) :
	PhStripPeopleObject(timeIn, people, timeOut, y, height), _content(content)
{
}

QString PhStripText::content() const
{
	return _content;
}

void PhStripText::setContent(QString content)
{
	_content = content;
}

