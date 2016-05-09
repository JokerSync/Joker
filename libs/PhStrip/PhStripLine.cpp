#include "PhStripLine.h"

PhStripLine::PhStripLine(PhTime timeIn, PhPeople *people, PhTime timeOut, float y, QString content, float height, bool selected) :
	PhStripPeopleObject(timeIn, people, timeOut, y, height),
	_content(content),
	_textModel(new PhStripTextModel()),
	_detectModel(new PhStripDetectModel())
{
	connect(_detectModel, PhStripDetectModel::rowsInserted, this, PhStripLine::refreshText);
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

void PhStripLine::refreshText()
{
	// refresh _textModel according to _content and _detectModel
	
	_textModel->clear();
	
	const int nbDetects = _detectModel->rowCount();
	
	PhTime startTime = timeIn();
	int startPosition = 0;

	for (int i = 0; i < nbDetects; ++i)
	{
		int position = _detectModel->index(i).data(PhStripDetectModel::PositionRole).toInt();

		if (position > 0) {
			PhTime detectTime = _detectModel->index(i).data(PhStripDetectModel::TimeInRole).toInt();
			QString content = _content.mid(startPosition, position - startPosition);

			PhStripText *text = new PhStripText(startTime, people(), detectTime, y(), content, height());
			_textModel->append(text);

			startTime = detectTime;
			startPosition = position;
		}
	}
	
	// add last piece of text
	QString content = _content.mid(startPosition, _content.length() - startPosition);
	PhStripText *text = new PhStripText(startTime, people(), timeOut(), y(), content, height());
	_textModel->append(text);
}
