#include "PhStripSentence.h"

PhStripSentence::PhStripSentence(PhStripText *text)
	: PhStripText(text->timeIn(), text->people(), text->timeOut(), text->y(), text->content(), text->height())
{
	_texts.append(text);
}

void PhStripSentence::add(PhStripText *text)
{
	_texts.append(text);
	this->setTimeOut(text->timeOut());
	this->setContent(this->content() + text->content());
}
