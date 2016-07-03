#ifndef PHSTRIPLINE_H
#define PHSTRIPLINE_H

#include <QString>
#include <QList>

#include "PhStrip/PhStripPeopleObject.h"
#include "PhStrip/PhStripTextModel.h"
#include "PhStrip/PhStripDetectModel.h"
#include "PhStrip/PhStripDetect.h"

class PhStripLine : public QObject
{
	Q_OBJECT

public:
	/**
	 * @brief PhStripLine constructor
	 * @param timeIn The line time in
	 * @param people The people affected to the line
	 * @param timeOut The text time out
	 * @param y The line vertical position
	 * @param content The line content
	 * @param height The line height
	 */
	PhStripLine(PhTime timeIn, PhStripDetect::PhDetectType typeIn, PhPeople * people, float y, float height);

	PhStripTextModel* textModel() {
		return _textModel;
	}

	PhStripDetectModel* detectModel() {
		return _detectModel;
	}

	PhTime timeIn() {
		return _timeIn;
	}

	void setTimeIn(PhTime timeIn);

	PhStripDetect::PhDetectType typeIn() const;

	void setTypeIn(const PhStripDetect::PhDetectType &typeIn);

	void append(PhStripText *text);

	void addUnlinkedDetect(PhStripDetect *detect);

	float y() const;
	void setY(float y);

	float height() const;
	void setHeight(float height);

	PhPeople *people() const;
	void setPeople(PhPeople *people);

	QString content();

	PhTime duration() const;

	PhTime timeOut() const;

signals:
	void timeOutChanged();

private slots:
	void updateTimeOut();

private:
	void setTimeOut(PhTime timeOut);

	PhStripDetectModel *_detectModel;
	PhStripTextModel *_textModel;
	PhTime _timeIn;
	PhTime _timeOut;
	PhStripDetect::PhDetectType _typeIn;
	PhPeople *_people;
	float _y;
	float _height;
};

#endif // PHSTRIPLINE_H
