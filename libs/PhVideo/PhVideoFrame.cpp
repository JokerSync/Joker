/**
 * @file
 * @copyright (C) 2012-2015 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhVideoFrame.h"

PhVideoFrame::PhVideoFrame() :
	_rgb(NULL),
	_size(0),
	_time(0),
	_width(0),
	_height(0)
{
}

PhVideoFrame::~PhVideoFrame()
{
	if (_rgb != NULL) {
		delete[] _rgb;
		_rgb = NULL;
	}
}

void PhVideoFrame::reuse(int size)
{
	if (_size != size) {
		// the size has changed, update the buffer
		if (_rgb != NULL) {
			delete[] _rgb;
		}
		_size = size;
		_rgb = new uint8_t[_size];
	}

	_time = 0;
}

uint8_t *PhVideoFrame::rgb()
{
	return _rgb;
}

PhTime PhVideoFrame::time()
{
	return _time;
}

PhTime PhVideoFrame::requestTime()
{
	return _requestTime;
}

int PhVideoFrame::width()
{
	return _width;
}

int PhVideoFrame::height()
{
	return _height;
}

void PhVideoFrame::setTime(PhTime time)
{
	_time = time;
}

void PhVideoFrame::setRequestTime(PhTime requestTime)
{
	_requestTime = requestTime;
}

void PhVideoFrame::setWidth(int width)
{
	_width = width;
}

void PhVideoFrame::setHeight(int height)
{
	_height = height;
}
