/**
 * @file
 * @copyright (C) 2012-2015 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhVideoBuffer.h"

PhVideoBuffer::PhVideoBuffer(int size) :
	_inUse(true),
	_size(size)
{
	_rgb = new uint8_t[size];
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
		_inUse = true;
	}

}

void PhVideoBuffer::recycle()
{
	_inUse = false;
}

bool PhVideoBuffer::isInUse()
{
	return _inUse;
}

uint8_t *PhVideoBuffer::rgb()
{
	return _rgb;
}
