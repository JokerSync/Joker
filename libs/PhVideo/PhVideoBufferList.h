/**
 * @file
 * @copyright (C) 2012-2015 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHVIDEOBUFFERLIST_H
#define PHVIDEOBUFFERLIST_H

#include "PhVideoBuffer.h"

/**
 * @brief A video buffer list
 *
 * It manages a pool of video buffers, allowing to reuse them if appropriate.
 */
class PhVideoBufferList
{
public:
	/**
	 * @brief PhVideoBufferList constructor
	 */
	PhVideoBufferList();

	~PhVideoBufferList();

	/**
	 * @brief Get a buffer, reuse existing one if possible, create a new one otherwise.
	 * @param bufferSize The buffer size
	 * @return A buffer
	 */
	PhVideoBuffer * newVideoBuffer(int bufferSize);

	/**
	 * @brief Clears the buffer list, delete all buffers it contains
	 */
	void clear();

	/**
	 * @brief Clears all unused buffers
	 */
	void clearUnusedBuffers();

private:
	QList<PhVideoBuffer*> _bufferList;
};

#endif // PHVIDEOBUFFERLIST_H
