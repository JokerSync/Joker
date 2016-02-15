/**
 * @file
 * @copyright (C) 2012-2015 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHVIDEOFRAME_H
#define PHVIDEOFRAME_H

#include "PhSync/PhClock.h"

/**
 * @brief A video frame
 *
 * It contains an rgb buffer (uint8 per pixel per color), and frame-related properties.
 */
class PhVideoFrame
{
public:
	/**
	 * @brief PhVideoFrame constructor
	 */
	PhVideoFrame();

	~PhVideoFrame();

	/**
	 * @brief Reuse an existing frame, recreating the rgb buffer if the new size is different.
	 * @param size The new size
	 */
	void reuse(int size);

	/**
	 * @brief Gets the rgb buffer
	 * @return The rgb buffer
	 */
	uint8_t *rgb();

	/**
	 * @brief Gets the time of the decoded frame (with origin at the start of video file)
	 * @return The time
	 */
	PhTime time();

	/**
	 * @brief Gets the request time of the frame (with origin at the start of video file)
	 * @return The time
	 */
	PhTime requestTime();

	/**
	 * @brief Gets the width of the frame
	 * @return The width of the frame
	 */
	int width();

	/**
	 * @brief Gets the height of the frame
	 * @return The height of the frame
	 */
	int height();

	/**
	 * @brief Sets the time of the decoded frame (with origin at the start of video file)
	 * @param time The time
	 */
	void setTime(PhTime time);

	/**
	 * @brief Sets the request time of the frame (with origin at the start of video file)
	 * @param requestTime The time
	 */
	void setRequestTime(PhTime requestTime);

	/**
	 * @brief Sets the width of the frame
	 * @param width The width of the frame
	 */
	void setWidth(int width);

	/**
	 * @brief Sets the height of the frame
	 * @param height The height of the frame
	 */
	void setHeight(int height);

private:
	uint8_t * _rgb;
	int _size;
	PhTime _requestTime;
	PhTime _time;
	int _width;
	int _height;
};

#endif // PHVIDEOFRAME_H
