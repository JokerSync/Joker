/**
 * @file
 * @copyright (C) 2012-2015 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhVideoBuffer.h"

#include "PhTools/PhDebug.h"

PhVideoBuffer::PhVideoBuffer() :
	_frame(0),
	_videoFrame(0),
	_bits(0),
	_bytesPerLine(0)
{
}

PhVideoBuffer::~PhVideoBuffer()
{
	if (_videoFrame != NULL) {
		delete _videoFrame;
		_videoFrame = NULL;
	}
}

void PhVideoBuffer::reuse(int size, int width, int height, int linesize, QVideoFrame::PixelFormat format)
{
	if (_videoFrame == NULL
			// bits cannot be read from QVideoFrame
			|| _bits != size
			|| _videoFrame->width() != width
			|| _videoFrame->height() != height
			// bytesPerLine cannot be read from QVideoFrame (unless mapped)
			|| _bytesPerLine != linesize
			|| _videoFrame->pixelFormat() != format) {

		// the parameters have changed, update the video frame
		if (_videoFrame != NULL) {
			delete _videoFrame;
		}

		PHDBG(24) << "PhVideoBuffer alloc" << size;
		_videoFrame = new QVideoFrame(size, QSize(width, height), linesize, format);
		_bits = size;
		_bytesPerLine = linesize;
	}

	_frame = 0;
}

QVideoFrame *PhVideoBuffer::videoFrame()
{
	return _videoFrame;
}

PhFrame PhVideoBuffer::frame()
{
	return _frame;
}

int PhVideoBuffer::width()
{
	if (_videoFrame == NULL) {
		return 0;
	}

	return _videoFrame->width();
}

int PhVideoBuffer::height()
{
	if (_videoFrame == NULL) {
		return 0;
	}

	return _videoFrame->height();
}

void PhVideoBuffer::setFrame(PhFrame frame)
{
	_frame = frame;
}
