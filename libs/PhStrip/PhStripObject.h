/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHSTRIPOBJECT_H
#define PHSTRIPOBJECT_H

#include <PhTools/PhClock.h>

/**
  * Generic object of a rythmo strip (loop, cut, text, ...).
  */
class PhStripObject {

private:
    /**
     * Starting time of the object.
     */
    PhTime _timeIn;
public:
    /**
     * @brief PhStripObject
     * default constructor
     */
    PhStripObject();
    /**
     * @brief PhStripObject
     * @param time
     * contructor
     */
    PhStripObject(PhTime time);
    /**
     * @brief getTimeIn
     * @return _timeIn
     */
    PhTime getTimeIn();
	/*!
	 * \brief Set the time in of the PhStripObject
	 * \param timeIn
	 */
	void setTimeIn(PhTime timeIn);
};

#endif // PHSTRIPOBJECT_H
