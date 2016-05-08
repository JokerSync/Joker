#include "PhStripLine.h"

PhStripLine::PhStripLine(PhTime timeIn, PhPeople *people, PhTime timeOut, float y, QString content, float height, bool selected) :
	PhStripPeopleObject(timeIn, people, timeOut, y, height),
	_content(content),
	_textModel(new PhStripTextModel()),
	_detectModel(new PhStripDetectModel())
{
}

QString PhStripLine::content() const
{
	return _content;
}

void PhStripLine::setContent(QString content)
{
	if (content != _content) {
		_content = content;
		emit contentChanged();
	}
}
