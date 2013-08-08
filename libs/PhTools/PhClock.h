#ifndef PHCLOCK_H
#define PHCLOCK_H

#include <QObject>
#include <QtWidgets>
#include <QString>
#include <Qtimer>
#include <QDebug>
#include <QGridLayout>
#include <QtGui>
#include "PhTools/PhTimeCode.h"
#include "PhTools/PhTime.h"
#include "PhTools/PhString.h"

using namespace std;

/**
 * The PhClock class modelize a clock with its current time and rate value.
 * It can be synchronized through an external signal.
 * It fires a signal when its time and rate value changes.
 */
class PhClock : QObject
{
	Q_OBJECT
public:
	explicit PhClock(QObject *parent = 0);

	//setters
	void set_rate(int rate);
	void set_timecode(PhFrame timecode);
	void set_timer(QTimer* timer);

	//getters
	float get_rate() const;
	PhFrame get_timecode() const;
	QTimer* get_timer() const;


signals:

public slots:

private:
	/**
	 * @brief _rateValue
	 * Value of the rate (0=pause, 1=play, 4=fastforward, -4=fastreward)
	 */
	float _rate;

	/**
	 * @brief _timer
	 */
	QTimer *_timer;

	/**
	 * @brief _timecode
	 * Standard timecode format "00:00:00:00"
	 */
	PhFrame _timecode;

};

#endif // PHCLOCK_H
