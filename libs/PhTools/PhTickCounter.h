#ifndef TICKCOUNTER_H
#define TICKCOUNTER_H

#include <QTime>

/**
 * TickCounter is an utility that allow to meter the frequency of a signal
 * connected to the tick slot.
 */
class PhTickCounter
{
public:
	PhTickCounter();

	float frequency() { return _frequency; }

public slots:
	void tick();

private:
	QTime _timer;
	float _frequency;
};

#endif // TICKCOUNTER_H
