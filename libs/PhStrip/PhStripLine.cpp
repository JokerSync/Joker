#include "PhStripLine.h"

#include "PhTools/PhDebug.h"

PhStripLine::PhStripLine(PhTime timeIn, PhStripDetect::PhDetectType typeIn, PhPeople *people, float y, float height) :
	_timeIn(timeIn),
	_typeIn(typeIn),
	_people(people),
	_y(y),
	_height(height),
	_textModel(new PhStripTextModel(this)),
	_detectModel(new PhStripDetectModel(this))
{
	//_textModel->append(text);

//	connect(_detectModel, PhStripDetectModel::rowsInserted, this, PhStripLine::onDetectInserted);
//	connect(_detectModel, PhStripDetectModel::rowsRemoved, this, PhStripLine::onDetectRemoved);
//	connect(_detectModel, PhStripDetectModel::dataChanged, this, PhStripLine::onDetectChanged);

//	connect(_textModel, PhStripTextModel::dataChanged, this, PhStripLine::onTextChanged);

	//connect(this, PhStripLine::contentChanged, this, PhStripLine::refreshText);
//	connect(this, PhStripLine::timeInChanged, this, PhStripLine::onTimeInChanged);
//	connect(this, PhStripLine::timeOutChanged, this, PhStripLine::onTimeOutChanged);
	//connect(this, PhStripLine::yChanged, this, PhStripLine::onYChanged);
}

//void PhStripLine::refreshText()
//{
//	// refresh _textModel according to _content and _detectModel
	
//	_textModel->clear();
	
//	const int nbDetects = _detectModel->rowCount();
	
//	PhTime startTime = timeIn();
//	int startPosition = 0;

//	for (int i = 0; i < nbDetects; ++i)
//	{
//		int position = _detectModel->index(i).data(PhStripDetectModel::PositionRole).toInt();

//		if (position > 0) {
//			PhTime detectTime = _detectModel->index(i).data(PhStripDetectModel::TimeInRole).toInt();
//			QString content = _content.mid(startPosition, position - startPosition);

//			PhStripText *text = new PhStripText(startTime, people(), detectTime, y(), content, height());
//			_textModel->append(text);

//			startTime = detectTime;
//			startPosition = position;
//		}
//	}
	
//	// add last piece of text
//	QString content = _content.mid(startPosition, _content.length() - startPosition);
//	PhStripText *text = new PhStripText(startTime, people(), timeOut(), y(), content, height());
//	_textModel->append(text);
//}

//void PhStripLine::onDetectInserted(const QModelIndex &parent, int first, int last)
//{
//	PHDEBUG << "detect inserted";
	
//	const int nbText = _textModel->rowCount();
//	const int nbDetect = _detectModel->rowCount();

//	for (int i=first; i<=last; i++) {
//		PhTime textTimeIn;
//		PhTime textTimeOut;
//		int textIndex;

//		PhTime newDetectTime = _detectModel->index(i).data(PhStripDetectModel::TimeInRole).toInt();
//		int newPosition = _detectModel->index(i).data(PhStripDetectModel::PositionRole).toInt();

//		// the new detect splits an existing text into 2
//		// find this text
//		for (int j = 0; j < nbText; ++j) {
//			textTimeIn = _textModel->index(j).data(PhStripTextModel::TimeInRole).toInt();
//			textTimeOut = _textModel->index(j).data(PhStripTextModel::TimeOutRole).toInt();
//			if (textTimeIn < newDetectTime && textTimeOut > newDetectTime) {
//				textIndex = j;
//				break;
//			}
//		}

//		PhTime previousDetectTime = 0;
//		int previousDetectPosition = 0;
//		PhTime nextDetectTime = timeOut();
//		int nextDetectPosition = _content.length();

//		// find the previous and next detects, if any
//		for (int k = 0; k < nbDetect; ++k) {
//			PhTime detectTime = _detectModel->index(k).data(PhStripDetectModel::TimeInRole).toInt();
//			int detectPosition = _detectModel->index(k).data(PhStripDetectModel::PositionRole).toInt();

//			if (detectTime < newDetectTime && detectTime > previousDetectTime) {
//				previousDetectTime = detectTime;
//				previousDetectPosition = detectPosition;
//			}

//			if (detectTime > newDetectTime && detectTime < nextDetectTime) {
//				nextDetectTime = detectTime;
//				nextDetectPosition = detectPosition;
//			}
//		}

//		// update the existing piece of text
//		QString content = _content.mid(previousDetectPosition, newPosition - previousDetectPosition);
//		_textModel->setData(_textModel->index(textIndex), newDetectTime, PhStripTextModel::TimeOutRole);
//		_textModel->setData(_textModel->index(textIndex), content, PhStripTextModel::ContentRole);

//		// add the new piece of text
//		QString newContent = _content.mid(newPosition, nextDetectPosition - newPosition);
//		PhStripText *text = new PhStripText(newDetectTime, people(), nextDetectTime, y(), newContent, height());
//		_textModel->append(text);
//	}
//}

//void PhStripLine::onDetectRemoved(const QModelIndex &parent, int first, int last)
//{
//	PHDEBUG << "detect removed";
//}

//void PhStripLine::onDetectChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
//{
//	PHDEBUG << "detect changed " + topLeft.row();

//	const int nbText = _textModel->rowCount();
//	const int nbDetect = _detectModel->rowCount();

//	// previous and next texts need to be updated
//	PhTime textTimeIn;
//	PhTime textTimeOut;
//	int previousTextIndex;
//	int nextTextIndex;
//	PhTime previousTextTimeIn = -1;
//	PhTime nextTextTimeOut = timeOut()+1;

//	PhTime newDetectTime = topLeft.data(PhStripDetectModel::TimeInRole).toInt();
//	int newPosition = topLeft.data(PhStripDetectModel::PositionRole).toInt();

//	for (int j = 0; j < nbText; ++j) {
//		textTimeIn = _textModel->index(j).data(PhStripTextModel::TimeInRole).toInt();
//		textTimeOut = _textModel->index(j).data(PhStripTextModel::TimeOutRole).toInt();

//		if (textTimeIn < newDetectTime && textTimeIn > previousTextTimeIn) {
//			previousTextIndex = j;
//		}

//		if (textTimeOut > newDetectTime && textTimeOut < nextTextTimeOut) {
//			nextTextIndex = j;
//		}
//	}

//	PhTime previousDetectTime = 0;
//	PhTime nextDetectTime = timeOut();
//	int previousDetectPosition = 0;
//	int nextDetectPosition = _content.length();

//	// find the previous and next detects, if any
//	for (int k = 0; k < nbDetect; ++k) {
//		PhTime detectTime = _detectModel->index(k).data(PhStripDetectModel::TimeInRole).toInt();
//		int detectPosition = _detectModel->index(k).data(PhStripDetectModel::PositionRole).toInt();

//		if (detectTime < newDetectTime && detectTime > previousDetectTime) {
//			previousDetectTime = detectTime;
//			previousDetectPosition = detectPosition;
//		}

//		if (detectTime > newDetectTime && detectTime < nextDetectTime) {
//			nextDetectTime = detectTime;
//			nextDetectPosition = detectPosition;
//		}
//	}

//	if (roles.contains(PhStripDetectModel::PositionRole)) {
//		// position changed => update PhStripText::content for previous and next
//		QString previousContent = _content.mid(previousDetectPosition, newPosition - previousDetectPosition);
//		QString nextContent = _content.mid(newPosition, nextDetectPosition - newPosition);
//		_textModel->setData(_textModel->index(previousTextIndex), previousContent, PhStripTextModel::ContentRole);
//		_textModel->setData(_textModel->index(nextTextIndex), nextContent, PhStripTextModel::ContentRole);
//	}

//	if (roles.contains(PhStripDetectModel::TimeInRole)) {
//		// time changed => update PhStripText::timeOut/In for previous/next texts
//		_textModel->setData(_textModel->index(previousTextIndex), newDetectTime, PhStripTextModel::TimeOutRole);
//		_textModel->setData(_textModel->index(nextTextIndex), newDetectTime, PhStripTextModel::TimeInRole);
//	}
//}

//void PhStripLine::onTextChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
//{
//	// content changed => update PhStripLine::content and positions of all next detects
//	// all other changes => do nothing
//	if (!roles.contains(PhStripTextModel::ContentRole)) {
//		return;
//	}
	
//	PhTime textTimeIn = topLeft.data(PhStripTextModel::TimeInRole).toInt();
//	PhTime textTimeOut = topLeft.data(PhStripTextModel::TimeOutRole).toInt();
//	QString textContent = topLeft.data(PhStripTextModel::ContentRole).toString();
	
//	int previousDetectIndex = -1;
//	int nextDetectIndex = -1;
//	int previousDetectPosition = 0;
//	int nextDetectPosition = _content.length() - 1;

//	// find the previous and next detects, if any
//	for (int k = 0; k < nbDetect; ++k) {
//		PhTime detectTime = _detectModel->index(k).data(PhStripDetectModel::TimeInRole).toInt();
//		int detectPosition = _detectModel->index(k).data(PhStripDetectModel::PositionRole).toInt();

//		if (detectTime == textTimeIn) {
//			previousDetectIndex = k;
//			previousDetectPosition = detectPosition;
//		}

//		if (detectTime == textTimeOut) {
//			nextDetectIndex = k;
//			nextDetectPosition = detectPosition;
//		}
//	}

//	// update PhStripLine::content
//	_content = _content.mid(0, previousDetectPosition) + textContent + _content.mid(nextDetectPosition, _content.length() - nextDetectPosition);

//	// update position of all next detects!
//}

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
