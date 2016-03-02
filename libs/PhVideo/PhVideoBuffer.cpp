/**
 * @file
 * @copyright (C) 2012-2015 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhVideoBuffer.h"

PhVideoBuffer::PhVideoBuffer() :
	_rgb(NULL),
	_size(0),
	_frame(0),
	_width(0),
	_height(0)
{
}

PhVideoBuffer::~PhVideoBuffer()
{
	if (_rgb != NULL) {
		delete[] _rgb;
		_rgb = NULL;
	}
}

void PhVideoBuffer::reuse(int size)
{
	if (_size != size) {
		// the size has changed, update the buffer
		if (_rgb != NULL) {
			delete[] _rgb;
		}
		_size = size;
		_rgb = new uint8_t[_size];
	}

	_frame = 0;
}

uint8_t *PhVideoBuffer::rgb()
{
	return _rgb;
}

PhFrame PhVideoBuffer::frame()
{
	return _frame;
}

PhFrame PhVideoBuffer::requestFrame()
{
	return _requestFrame;
}

int PhVideoBuffer::width()
{
	return _width;
}

int PhVideoBuffer::height()
{
	return _height;
}

void PhVideoBuffer::setFrame(PhFrame frame)
{
	_frame = frame;
}

void PhVideoBuffer::setRequestFrame(PhFrame requestFrame)
{
	_requestFrame = requestFrame;
}

void PhVideoBuffer::setWidth(int width)
{
	_width = width;
}

void PhVideoBuffer::setHeight(int height)
{
	_height = height;
}
