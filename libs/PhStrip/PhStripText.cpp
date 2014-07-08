/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStripText.h"



PhStripText::PhStripText(PhTime timeIn, PhPeople *people, PhTime timeOut, float track, QString content, float height) :
	PhStripPeopleObject(timeIn, people, timeOut, track, height), _content(content)
{
}

QString PhStripText::content()
{
	return _content;
}

