#include "PhStripSentence.h"

PhStripSentence::PhStripSentence(PhStripText *text) : PhStripPeopleObject(text->timeIn(), text->people(), text->timeOut(), text->y(), text->height())
{

}
