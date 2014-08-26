/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSTRIPTEXT_H
#define PHSTRIPTEXT_H

#include "PhPeople.h"
#include "PhStrip/PhStripPeopleObject.h"
#include "PhStrip/PhStripDetect.h"

/**
 * @brief Sentence or part of a sentence of a PhPeople with synchronization information.
 *
 * Its property is content.
 */
class PhStripText : QObject
{

	Q_OBJECT

public:

	/**
	 * @brief PhStripText constructor
	 * @param timeIn The text time in
	 * @param people The people affected to the text
	 * @param timeOut The text time out
	 * @param y The text vertical position
	 * @param content The text content
	 * @param height The text height
	 */
	PhStripText(QString content, PhTime duration, PhStripDetect::PhDetectType typeOut);
	/**
	 * @brief Get the text content
	 * @return A string
	 */
	QString content() const;

	/**
	 * @brief Set the text content
	 * @param content a string
	 */
	void setContent(QString content);

	PhStripDetect::PhDetectType typeOut() const;
	void setTypeOut(const PhStripDetect::PhDetectType &typeOut);

	PhTime duration() const;
	void setDuration(const PhTime &duration);

signals:
	void contentChanged();
private:
	QString _content;
	PhTime _duration;
	PhStripDetect::PhDetectType _typeOut;
};

#endif // PHSTRIPTEXT_H
