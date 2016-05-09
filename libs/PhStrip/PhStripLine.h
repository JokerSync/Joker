#ifndef PHSTRIPLINE_H
#define PHSTRIPLINE_H

#include <QString>
#include <QList>

#include "PhStrip/PhStripPeopleObject.h"
#include "PhStrip/PhStripTextModel.h"
#include "PhStrip/PhStripDetectModel.h"

class PhStripLine : public PhStripPeopleObject
{
	Q_OBJECT

	Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)

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
	PhStripLine( PhTime timeIn, PhPeople * people, PhTime timeOut, float y,  QString content, float height, bool selected = true);

	PhStripTextModel* textModel() {
		return _textModel;
	}

	PhStripDetectModel* detectModel() {
		return _detectModel;
	}

	/**
	 * @brief Get the text content
	 * @return _content
	 */
	QString content() const;
	/**
	 * @brief Set the text content
	 * @param content a string
	 */
	void setContent(QString content);

public slots:
	void refreshText();

signals:
	void contentChanged();

private:
	QString _content;

	PhStripDetectModel *_detectModel;

	PhStripTextModel *_textModel;
};

#endif // PHSTRIPLINE_H
