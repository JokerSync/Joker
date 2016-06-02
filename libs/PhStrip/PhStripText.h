/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSTRIPTEXT_H
#define PHSTRIPTEXT_H

#include "PhPeople.h"
#include "PhStrip/PhStripPeopleObject.h"

/**
 * @brief Sentence or part of a sentence of a PhPeople with synchronization information.
 *
 * Its property is content.
 */
class PhStripText : public PhStripPeopleObject {

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
	PhStripText( PhTime timeIn, PhPeople * people, PhTime timeOut, float y,  QString content, float height);
	/**
	 * @brief Get the text content
	 * @return A string
	 */
	QString content() const;

	/**
	 * @brief Set the text content
	 * @param content A string
	 */
	void setContent(QString content);

	/**
	 * @brief Text description
	 * @param tcType The timecode type use to display
	 * @return A string
	 */
	QString description(PhTimeCodeType tcType);
private:

/**
 * Text string of Sentence or part of a sentence.
 */
	QString _content;
};

#endif // PHSTRIPTEXT_H
