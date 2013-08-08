#ifndef PHCLOCK_H
#define PHCLOCK_H

#include <QObject>

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

signals:

public slots:
};

#endif // PHCLOCK_H
