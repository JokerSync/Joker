#ifndef PHGRAPHICDISC_H
#define PHGRAPHICDISC_H

#include "PhGraphicObject.h"

/**
 * @brief A graphic disc
 *
 * A PhGraphicDisc is described by :
 * - its position (x, y, z)
 * - its radius
 * - its resolution
 */
class PhGraphicDisc : public PhGraphicObject
{
public:
	/**
	 * @brief PhGraphicDisc
	 * @param x
	 * @param y
	 * @param radius
	 * @param resolution
	 */
	PhGraphicDisc(int x = 0, int y = 0, int radius = 0, int resolution = 36);

	/**
	 * @brief Get the radius
	 * @return the radius size in pixel
	 */
	int radius() {
		return _radius;
	}
	/**
	 * @brief Set the radius size in pixel
	 * @param radius
	 */
	void setRadius(int radius) {
		_radius = radius;
	}
	/**
	 * @brief resolution
	 * @return
	 */
	int resolution() {
		return _resolution;
	}
	/**
	 * @brief setResolution
	 * @param resolution
	 */
	void setResolution(int resolution) {
		_resolution = resolution;
	}

	void draw();

private:
	int _radius;
	int _resolution;

};

#endif // PHGRAPHICDISC_H
