/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTestTools.h"

PhTestTools::PhTestTools()
{
}

bool PhTestTools::compareFloats(float a, float b, float epsilon)
{
	return (a - b) < epsilon && (b - a) < epsilon;
}
