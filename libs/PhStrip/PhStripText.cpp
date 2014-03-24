/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStripText.h"



PhStripText::PhStripText(PhFrame frameIn, PhPeople *people, PhFrame frameOut, int track, QString content) :
	PhStripPeopleObject(frameIn, people, frameOut, track), _content(content)
{
}

QString PhStripText::content()
{
	return _content;
}

