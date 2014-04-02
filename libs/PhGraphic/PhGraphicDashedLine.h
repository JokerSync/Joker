#ifndef PHGRAPHICDASHEDLINE_H
#define PHGRAPHICDASHEDLINE_H

#include "PhGraphicRect.h"

/**
 * @brief A graphic horizontal dashed line
 *
 * This graphic line contains a serie of rectangle filled by an solid color and specified by
 * a color, position and size (inherited from PhGraphicRect)
 */
class PhGraphicDashedLine : public PhGraphicRect
{
public:
	/**
	 * @brief PhGraphicDashedLine constructor
	 * @param dashCount The number of dash
	 * @param x The x coordinate
	 * @param y The y coordinate
	 * @param w The width
	 * @param h The height
	 */
	PhGraphicDashedLine(int dashCount = 1, int x = 0, int y = 0, int w = 0, int h = 0);

	/**
	 * @brief Set the number of dash
	 * @param dashCount The number of dash
	 */
	void setDashCount(int dashCount);

	/**
	 * @brief Get the number of dash
	 * @return The number of dash
	 */
	int dashCount() {
		return _dashCount;
	}

	/**
	 * @brief Draw a dashed line
	 */
	void draw();

private:
	int _dashCount;
};

#endif // PHGRAPHICDASHEDLINE_H
