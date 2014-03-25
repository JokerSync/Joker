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
	 * @param type The cut type
	 * @param frame The cut frame
	 */
	PhStripCut(PhStripCut::PhCutType type, PhFrame frame);


private:
	/**
	 * Type of cut.
	 */
	PhCutType _type;

};


#endif // PHSTRIPCUT_H
