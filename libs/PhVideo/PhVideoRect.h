#ifndef PHVIDEORECT_H
#define PHVIDEORECT_H

#include "PhGraphic/PhGraphicTexturedRect.h"

#include "PhVideoBuffer.h"

/**
 * @brief The rectangle area on which will be displayed the video texture
 */
class PhVideoRect : public PhGraphicTexturedRect
{
public:
	/**
	 * @brief The PhVideoRect constructor
	 */
	PhVideoRect();

	/**
	 * @brief Update the video texture with the buffer content
	 * @param buffer A PhVideoBuffer containing RGB data
	 */
	void update(PhVideoBuffer *buffer);

	/**
	 * @brief Current frame number contained in the texture
	 * @return A frame number
	 */
	PhFrame currentFrame() const;

	/**
	 * @brief Discard the current frame number
	 */
	void discard();
private:
	PhFrame _currentFrame;
};

#endif // PHVIDEORECT_H
