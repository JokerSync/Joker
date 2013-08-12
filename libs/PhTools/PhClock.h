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
class PhClock : public QObject
{
	Q_OBJECT
public:
	explicit PhClock(QObject *parent = 0);

	//setters
	void setRate(float rate);
	void setFrame(PhFrame frame);

	//getters
	float getRate() const;
	PhFrame getFrame() const;

	~PhClock();


signals:

	void frameChanged();
	void rateChanged();

public slots:

	void tick();


private:

	float _rate;
	PhFrame _frame;

};

#endif // PHCLOCK_H
