/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSTRIPCUT_H
#define PHSTRIPCUT_H

#include "PhStrip/PhStripObject.h"

/**
 * @brief A cut is a change in the video from one shot to another.
 *
 * It can be simple (one frame change) or progressive (fade).
 */
class PhStripCut : public PhStripObject {

public:
	/**
	 * Describes the type of the cut.
	 */
	enum PhCutType {
		Simple, FadeIn, FadeOut, CrossFade
	};

	/**
	 * @brief PhStripCut constructor
	 * @param time The cut time
	 * @param type The cut type
	 */
	PhStripCut(PhTime time, PhStripCut::PhCutType type);

	/**
	 * @brief Get the cut type
	 * @return A cut type
	 */
	PhCutType type() const {
		return _type;
	}
private:
	/**
	 * Type of cut.
	 */
	PhCutType _type;

};


#endif // PHSTRIPCUT_H
