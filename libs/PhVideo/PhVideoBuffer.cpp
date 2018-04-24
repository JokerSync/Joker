/**
 * @file
 * @copyright (C) 2012-2015 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhVideoBuffer.h"

#include "PhTools/PhDebug.h"

PhVideoBuffer::PhVideoBuffer(int width, int height, QVideoFrame::PixelFormat format, AVPixelFormat pix_format)
	: _frame(0)
{
	PHDBG(24) << "PhVideoBuffer alloc" << width << height << format;
	_planarVideoBuffer = new PhPlanarVideoBuffer(pix_format, width, height);
	_videoFrame = new QVideoFrame(_planarVideoBuffer, QSize(width, height), format);
}

PhVideoBuffer::~PhVideoBuffer()
{
	if (_videoFrame != NULL) {
		// this also deletes _planarVideoBuffer
		delete _videoFrame;
		_videoFrame = NULL;
	}
}

void PhVideoBuffer::setAvFrame(AVFrame *avFrame)
{
	_planarVideoBuffer->setAvFrame(avFrame);
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
