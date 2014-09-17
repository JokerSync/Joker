/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHSTRIPOBJECT_H
#define PHSTRIPOBJECT_H

#include "PhSync/PhClock.h"

/**
 * @brief Generic object of a rythmo strip (loop, cut, text, ...).
 *
 * Its only property is timeIn.
 */
class PhStripObject : public QObject {

	Q_OBJECT

	Q_PROPERTY(PhTime timeIn READ timeIn WRITE setTimeIn NOTIFY timeInChanged)

private:
	/**
	 * Starting time of the object.
	 */
	PhTime _timeIn;
public:
	/**
	 * @brief PhStripObject constructor
	 * @param timeIn The time in
	 */
	PhStripObject(PhTime timeIn);
	/**
	 * @brief ~PhStripObject
	 */
	virtual ~PhStripObject() {
	}
	/**
	 * @brief The time in
	 * @return A PhTime
	 */
	PhTime timeIn();

	/**
	 * @brief Set the time in
	 * @param timeIn a PhTime
	 */
	void setTimeIn(PhTime timeIn);

	/**
	 * @brief Compare two strip object based on the time in
	 *
	 * @param a A strip object
	 * @param b Another strip object
	 * @return True if "a" has a stricly lower time in than "b", false otherwise.
	 */
	static bool dtcomp(PhStripObject *a, PhStripObject *b);

signals:
	void timeInChanged();
};

#endif // PHSTRIPOBJECT_H
