#ifndef PHGRAPHICDISC_H
#define PHGRAPHICDISC_H

#include "PhGraphicObject.h"

class PhGraphicDisc : public PhGraphicObject
{
public:
	PhGraphicDisc(int x = 0, int y = 0, int radius = 0, int resolution = 36);

	int radius() { return _radius;}

	void setRadius(int radius) { _radius = radius;}

	int resolution() { return _resolution;}

	void setResolution(int resolution) { _resolution = resolution;}

	void draw();

private:
	int _radius;
	int _resolution;

};

#endif // PHGRAPHICDISC_H
