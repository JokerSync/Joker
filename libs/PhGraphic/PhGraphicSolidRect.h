/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICSOLIDSQUARE_H
#define PHGRAPHICSOLIDSQUARE_H

#include "PhGraphicRect.h"
#include <glu.h>

class PhGraphicSolidRect : public PhGraphicRect
{
public:

	PhGraphicSolidRect(int x = 0, int y =0, int w = 0, int h = 0);
	~PhGraphicSolidRect();

	void draw();

};

#endif // PHGRAPHICSOLIDSQUARE_H
