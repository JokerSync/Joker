/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSTRIPLOOP_H
#define PHSTRIPLOOP_H

#include "PhStripObject.h"

/**
 * @brief Indicate the start of a new loop.
 *
 * A loop is in fact a section of the rythmo strip.
 * The detection process consists to cut the video content into loops to simplify
 * the convocation of the dubbing actors for recording.
 * It is generally between 30 seconds and 1 minute long: what the actor can record in one take.
 */
class PhStripLoop : public PhStripObject
{
public:
	/**
	 * @brief PhStripLoop constructor
	 * @param timeIn The loop time
	 * @param label The loop label
	 */
	PhStripLoop(PhTime timeIn, QString label);

	/**
	 * \brief Get the loop label
	 *
	 * The loop label can ever be a number or a text.
	 *
	 * \return A string value
	 */
	QString label() {
		return _label;
	}

	void setLabel(const QString &label);

	/**
	 * @brief Loop description
	 * @param tcType The timecode type use to display
	 * @return A string
	 */
	QString description(PhTimeCodeType tcType);

private:
	QString _label;
};


#endif // PHSTRIPLOOP_H
