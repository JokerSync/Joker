/**
 * @file
 * @copyright (C) 2012-2015 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHVIDEOBUFFER_H
#define PHVIDEOBUFFER_H

#include <QVideoFrame>
#include "PhSync/PhTime.h"
#include "PhPlanarVideoBuffer.h"

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
	 * @param width The width
	 * @param height The height
	 * @param format The pixel format
	 * @param pix_format The ffmpeg pixel format
	 */
	PhVideoBuffer(int width, int height, QVideoFrame::PixelFormat format, AVPixelFormat pix_format);

	~PhVideoBuffer();

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

	void setAvFrame(AVFrame *avFrame);

private:
	PhFrame _frame;
	QVideoFrame * _videoFrame;
	PhPlanarVideoBuffer * _planarVideoBuffer;
};

Q_DECLARE_METATYPE(PhVideoBuffer*)

#endif // PHVIDEOBUFFER_H
