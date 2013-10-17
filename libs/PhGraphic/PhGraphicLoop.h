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

	PhGraphicLoop(int x = 0, int y =0, int w = 0, int h = 0, int crossHeight = 0, int hThick = 0);

	void draw();

	void setHThick(int hThick);
	int getHThick(){return _hThick;}

	void setCrossHeight(int crossHeight);
	int getCrossHeight(){return _crossHeight;}
private:

	int _crossHeight;
	int _hThick;

};
#endif // PHGRAPHICLOOP_H
