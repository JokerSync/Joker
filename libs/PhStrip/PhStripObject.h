/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSTRIPOBJECT_H
#define PHSTRIPOBJECT_H

#include <PhTools/PhClock.h>

/**
 * @brief Generic object of a rythmo strip (loop, cut, text, ...).
 *
 * Its only property is frameIn.
 */
class PhStripObject {

private:
	/**
	 * Starting frame of the object.
	 */
	PhFrame _frameIn;
public:
	/**
	 * @brief PhStripObject
	 * @param frameIn
	 * contructor
	 */
	PhStripObject(PhFrame frameIn);
	/**
	 * @brief The frame in
	 * @return A PhFrame
	 */
	PhFrame frameIn();
};

#endif // PHSTRIPOBJECT_H
