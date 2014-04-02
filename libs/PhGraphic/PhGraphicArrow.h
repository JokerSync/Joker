#ifndef PHGRAPHICARROW_H
#define PHGRAPHICARROW_H

#include "PhGraphicRect.h"

/**
 * @brief A graphic diagonal arrow
 */
class PhGraphicArrow : public PhGraphicRect
{
public:
	/**
	 * @brief The PhGraphicArrowDirection enum
	 *
	 * This graphic arrow can point
	 * - from the down/left corner to the upper/right corner
	 * - from the upper/left corner to the down/right corner
	 */
	enum PhGraphicArrowDirection {
		DownLeftToUpRight,
		UpLefToDownRight,
	};

	/**
	 * @brief PhGraphicArrow constructor
	 * @param direction The direction
	 * @param x The x coordinate
	 * @param y The y coordinate
	 * @param w The width
	 * @param h The height
	 */
	PhGraphicArrow(PhGraphicArrowDirection direction = DownLeftToUpRight, int x = 0, int y = 0, int w = 0, int h = 0);

	/**
	 * @brief Set the direction
	 * @param direction The direction
	 */
	void setDirection(PhGraphicArrowDirection direction);

	/**
	 * @brief Get the direction
	 * @return The direction
	 */
	int direction() {
		return _direction;
	}

	/**
	 * @brief Draw the arrow
	 */
	void draw();

private:
	PhGraphicArrowDirection _direction;
};

#endif // PHGRAPHICARROW_H
