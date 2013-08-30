/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICLOOP_H
#define PHGRAPHICLOOP_H

#include "PhGraphicRect.h"
#include <glu.h>
#include <QDebug>

class PhGraphicLoop : public PhGraphicRect
{
public:

	PhGraphicLoop(float x = 0, float y =0, float w = 0, float h = 0, float crossHeight = 0, float hThick = 0, float z = 0, QColor *color = new QColor());

	bool init();

	void draw();

	void setHThick(float hThick);
	float getHThick(){return _hThick;};

	void setCrossHeight(float crossHeight);
	float getCrossHeight(){return _crossHeight;};
private:

	float _hThick;
	float _crossHeight;

};
#endif // PHGRAPHICLOOP_H
