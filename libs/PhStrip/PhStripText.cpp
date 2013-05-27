#include "PhStripText.h"


PhStripText::PhStripText(PhPeople *people, QString content, int timeIn, int timeOut) : PhStripObject(timeIn)
{
    _people = people;
    _timeOut = timeOut;
    _content = content ;
}
