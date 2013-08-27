/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHGRAPHICSOLIDSQUARE_H
#define PHGRAPHICSOLIDSQUARE_H

#include "PhGraphicRect.h"
#include <glu.h>

class PhGraphicSolidRect : public PhGraphicRect
{
public:

	PhGraphicSolidRect(float x = 0, float y =0, float w = 0, float h = 0, float z = 0, QColor *color = new QColor());

	bool init();

	void draw();

};

#endif // PHGRAPHICSOLIDSQUARE_H
