/**
 * @file
 * @copyright (C) 2012-2015 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHVIDEOBUFFER_H
#define PHVIDEOBUFFER_H

#include <QVideoFrame>
#include "PhSync/PhTime.h"

/**
 * @brief A video buffer
 *
 * It contains an rgb buffer (uint8 per pixel per color), and frame-related properties.
 */
class PhVideoBuffer
{
public:
	/**
	 * @brief PhVideoBuffer constructor
	 */
	PhVideoBuffer();

	~PhVideoBuffer();

	/**
	 * @brief Reuse an existing buffer, recreating the rgb buffer if the new size is different.
	 * @param size The new size
	 */
	//void reuse(int size);
	void reuse(int size, int width, int height, int linesize, QVideoFrame::PixelFormat format);

	/**
	 * @brief Gets the video buffer
	 * @return The video frame
	 */
	QVideoFrame * videoFrame();

	/**
	 * @brief Gets the frame number of the decoded buffer (with origin at the start of video file)
	 * @return The frame number
	 */
	PhFrame frame();

	/**
	 * @brief Gets the width of the buffer
	 * @return The width of the buffer
	 */
	int width();

	/**
	 * @brief Gets the height of the buffer
	 * @return The height of the buffer
	 */
	int height();

	/**
	 * @brief Sets the frame number of the decoded buffer (with origin at the start of video file)
	 * @param frame The frame number
	 */
	void setFrame(PhFrame frame);

private:
	PhFrame _frame;
	QVideoFrame * _videoFrame;
};

#endif // PHVIDEOBUFFER_H
