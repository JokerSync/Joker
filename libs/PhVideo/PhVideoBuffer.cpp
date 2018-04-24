/**
 * @file
 * @copyright (C) 2012-2015 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QThread>
#include "PhVideoBuffer.h"

#include "PhTools/PhDebug.h"

PhVideoBuffer::PhVideoBuffer(int size, int width, int height, int linesize, QVideoFrame::PixelFormat format) :
	_frame(0),
	_videoFrame(NULL)
{
	PHDBG(24) << "PhVideoBuffer alloc" << size << width << height << linesize << format;
	try {
	  _videoFrame = new QVideoFrame(size, QSize(width, height), linesize, format);
	}
	catch (const std::bad_alloc&) {
		PHDEBUG << "Failed to allocate, Joker will crash after 120 seconds (to have time for a dump)";
		QThread::sleep(120);
		std::abort();
	}
}

PhVideoBuffer::~PhVideoBuffer()
{
	if (_videoFrame != NULL) {
		delete _videoFrame;
		_videoFrame = NULL;
	}
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
