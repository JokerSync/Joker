#ifndef PHNEXTPEOPLE_H
#define PHNEXTPEOPLE_H

#include "PhStrip/PhPeople.h"
#include "PhSync/PhTime.h"

class PhNextPeople : public PhPeople
{
	Q_OBJECT
	Q_PROPERTY(PhTime timeIn READ timeIn WRITE setTimeIn NOTIFY timeInChanged)
	Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)
	Q_PROPERTY(PhTime duration READ duration WRITE setDuration NOTIFY durationChanged)

public:
   /*
	* @brief PhNextPeople
	* @param name
	* @param color
	*/
   PhNextPeople(QString name = "???", QString color = "#000000", PhTime timeIn = 0, bool selected = true, PhTime duration = 0);

	/**
	 * @brief Get the time in
	 * @return a PhTime
	 */
	PhTime timeIn() const;
	/**
	 * @brief Set the time in
	 * @param timeIn a PhTime
	 */
	void setTimeIn(PhTime timeIn);
	/**
	 * @brief Get the duration
	 * @return a PhTime
	 */
	PhTime duration() const;
	/**
	 * @brief Set the duration
	 * @param duration a PhTime
	 */
	void setDuration(PhTime duration);
	/**
	 * @brief Get whether this person is selected
	 * @return a bool
	 */
	bool selected() const;
	/**
	 * @brief Set whether this person is selected
	 * @param selected a bool
	 */
	void setSelected(bool selected);

signals:
	void timeInChanged();
	void selectedChanged();
	void durationChanged();

private:
	PhTime _timeIn;
	bool _selected;
	PhTime _duration;
};

#endif // PHNEXTPEOPLE_H
