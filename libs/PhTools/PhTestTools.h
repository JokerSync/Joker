/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHTESTTOOLS_H
#define PHTESTTOOLS_H

/**
 * @brief The PhTestTools class
 * This class provide awesome tools for unit testing
 */
class PhTestTools
{
public:
	PhTestTools();
	/**
	 * @brief compareFloats
	 * @param a the first float to compare
	 * @param b the second float to compare
	 * @param epsilon the max diff expected
	 * @return true is a and b differ of less than epsilon
	 */
	static bool compareFloats(float a, float b, float epsilon = 0.00000001);
};

#endif // PHTESTTOOLS_H
