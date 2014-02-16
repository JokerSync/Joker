/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStripText.h"



PhStripText::PhStripText(PhTime timeIn, PhPeople *people, PhTime timeOut, int track, QString content) :
	PhStripPeopleObject(timeIn, people, timeOut, track), _content(content)
{
}

QString PhStripText::getContent()
{
	return _content;
}

void PhStripText::setContent(QString content)
{
	_content = content;
}

