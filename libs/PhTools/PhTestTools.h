/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHTESTTOOLS_H
#define PHTESTTOOLS_H

class PhTestTools
{
public:
	PhTestTools();
	static bool compareFloats(float a, float b, float epsilon = 0.00000001);
};

#endif // PHTESTTOOLS_H
