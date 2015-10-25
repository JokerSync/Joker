/**
 * @file
 * @copyright (C) 2012-2015 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHVIDEOBUFFER_H
#define PHVIDEOBUFFER_H

#include "PhTools/PhGeneric.h"

/**
 * @brief A video buffer
 *
 * It contains an rgb buffer (uint8 per pixel per color).
 * The class using the buffer can mark that the buffer is not used anymore.
 * This allows to reuse existing buffers, instead of creating new ones.
 */
class PhVideoBuffer
{
public:
	/**
	 * @brief PhVideoBuffer constructor
	 * @param size The size of the rgb buffer
	 */
	PhVideoBuffer(int size);

	~PhVideoBuffer();

	/**
	 * @brief Reuse an existing buffer, recreating the rgb buffer only if the new size is different.
	 * @param size The new size
	 */
	void reuse(int size);

	/**
	 * @brief Recycle a buffer, marking it as unused
	 */
	void recycle();

	/**
	 * @brief Gets whether this buffer is in use
	 * @return Whether the buffer is in use
	 */
	bool isInUse();

	/**
	 * @brief Gets the rgb buffer
	 * @return The rgb buffer
	 */
	uint8_t *rgb();

private:
	uint8_t * _rgb;
	bool _inUse;
	int _size;
};

#endif // PHVIDEOBUFFER_H
