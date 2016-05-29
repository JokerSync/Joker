#ifndef PHSTRIPLINE_H
#define PHSTRIPLINE_H

#include <QString>
#include <QList>

#include "PhStrip/PhStripPeopleObject.h"
#include "PhStrip/PhStripTextModel.h"
#include "PhStrip/PhStripDetectModel.h"
#include "PhStrip/PhStripDetect.h"

class PhStripLine : QObject
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

	void setTimeIn(PhTime timeIn) {
		_timeIn = timeIn;
	}

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

public slots:
	//	void refreshText();

	//	void onTimeInChanged();

//	void onTimeOutChanged();

//	void onDetectInserted(const QModelIndex &parent, int first, int last);

//	void onDetectRemoved(const QModelIndex &parent, int first, int last);

//	void onDetectChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);

//	void onTextChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);

private:
	PhStripDetectModel *_detectModel;
	PhStripTextModel *_textModel;
	PhTime _timeIn;
	PhStripDetect::PhDetectType _typeIn;
	PhPeople *_people;
	float _y;
	float _height;
};

#endif // PHSTRIPLINE_H
