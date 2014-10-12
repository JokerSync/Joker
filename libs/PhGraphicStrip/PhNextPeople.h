#ifndef PHNEXTPEOPLE_H
#define PHNEXTPEOPLE_H

#include "PhStrip/PhPeople.h"
#include "PhSync/PhTime.h"

class PhNextPeople : public PhPeople
{
	Q_OBJECT
	Q_PROPERTY(PhTime timeIn READ timeIn WRITE setTimeIn NOTIFY timeInChanged)
	Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged)

public:
   /*
	* @brief PhNextPeople
	* @param name
	* @param color
	*/
   PhNextPeople(QString name = "???", QString color = "#000000", PhTime timeIn = 0, bool selected = true);

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

private:
	PhTime _timeIn;
	bool _selected;
};

#endif // PHNEXTPEOPLE_H
