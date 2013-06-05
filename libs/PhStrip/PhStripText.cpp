/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhStripText.h"


PhStripText::PhStripText(PhPeople *people, QString content, int timeIn, int timeOut, int track, bool simpleText) : PhStripObject(timeIn)
{
    _people = people;
    _timeOut = timeOut;
    _content = content ;
    _track = track;
    _simpleText = simpleText;
}

PhString PhStripText::getContent()
{
    return _content;
}

PhPeople PhStripText::getPeople()
{
    return *_people;
}

int PhStripText::getTrack()
{
    return _track;
}

int PhStripText::getTimeIn()
{
    return PhStripObject::getTimeIn();
}

int PhStripText::getTimeOut()
{
    return _timeOut;
}

bool PhStripText::isSimple(){
    return _simpleText;
}
