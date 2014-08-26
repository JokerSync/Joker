#include "PhStripLine.h"

#include "PhTools/PhDebug.h"

PhStripLine::PhStripLine(PhTime timeIn, PhStripDetect::PhDetectType typeIn, PhPeople *people, float y, float height) :
	PhStripObject(timeIn),
	_timeIn(timeIn),
	_timeOut(timeIn),
	_typeIn(typeIn),
	_people(people),
	_y(y),
	_height(height),
	_textModel(new PhStripTextModel(this)),
	_detectModel(new PhStripDetectModel(this))
{
	connect(_textModel, &PhStripTextModel::durationChanged, this, &PhStripLine::updateTimeOut);
}

void PhStripLine::setTimeIn(PhTime timeIn)
{
	if (timeIn != _timeIn) {
		_timeIn = timeIn;
		updateTimeOut();
	}
}

PhStripDetect::PhDetectType PhStripLine::typeIn() const
{
	return _typeIn;
}

void PhStripLine::setTypeIn(const PhStripDetect::PhDetectType &typeIn)
{
	_typeIn = typeIn;
}

void PhStripLine::append(PhStripText *text)
{
	_textModel->append(text);
}

void PhStripLine::addUnlinkedDetect(PhStripDetect *detect)
{
	_detectModel->append(detect);
}

float PhStripLine::y() const
{
	return _y;
}

void PhStripLine::setY(float y)
{
	_y = y;
}

float PhStripLine::height() const
{
	return _height;
}

void PhStripLine::setHeight(float height)
{
	_height = height;
}

PhPeople *PhStripLine::people() const
{
    return _people;
}

void PhStripLine::setPeople(PhPeople *people)
{
	_people = people;
}

QString PhStripLine::content()
{
	QString content = "";
	QListIterator<PhStripText*> i = _textModel->iterator();
	while (i.hasNext()) {
		content += i.next()->content();
	}
	return content;
}

PhTime PhStripLine::duration() const
{
	return _textModel->duration();
}

PhTime PhStripLine::timeOut() const
{
	return _timeOut;
}

void PhStripLine::setTimeOut(PhTime timeOut)
{
	if (timeOut != _timeOut) {
		_timeOut = timeOut;
		emit timeOutChanged();
	}
}

void PhStripLine::updateTimeOut()
{
	PhTime timeOut = timeIn() + duration();
	setTimeOut(timeOut);
}

QString PhStripLine::description(PhTimeCodeType tcType)
{
	return QString("%1 %2: %3")
		   .arg(this->people() ? this->people()->name() : "???")
		   .arg(this->tcIn(tcType))
		   .arg(this->content());
}
