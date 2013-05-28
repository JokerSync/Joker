#include "PhStripText.h"


PhStripText::PhStripText(PhPeople *people, QString content, int timeIn, int timeOut, int track) : PhStripObject(timeIn)
{
    _people = people;
    _timeOut = timeOut;
    _content = content ;
    _track = track;
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
