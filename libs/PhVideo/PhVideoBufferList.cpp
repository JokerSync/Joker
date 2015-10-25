/**
 * @file
 * @copyright (C) 2012-2015 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhVideoBufferList.h"

PhVideoBufferList::PhVideoBufferList()
{
}

PhVideoBufferList::~PhVideoBufferList()
{
	clear();
}

void PhVideoBufferList::clear()
{
	qDeleteAll(_bufferList);
	_bufferList.clear();
}

void PhVideoBufferList::clearUnusedBuffers()
{
	QMutableListIterator<PhVideoBuffer*> i(_bufferList);
	while (i.hasNext()) {
		PhVideoBuffer *buffer = i.next();
		if (buffer->isInUse() == false) {
			i.remove();
			delete buffer;
		}
	}
}

PhVideoBuffer * PhVideoBufferList::newVideoBuffer(int bufferSize)
{
	// find an available buffer, reuse existing one if possible

	foreach (PhVideoBuffer *existingBuffer, _bufferList) {
		if (existingBuffer->isInUse() == false) {
			// we can reuse an existing available buffer
			existingBuffer->reuse(bufferSize);
			return existingBuffer;
		}
	}

	// no buffer is currently available, we need a new one
	PhVideoBuffer *newBuffer = new PhVideoBuffer(bufferSize);
	_bufferList.append(newBuffer);
	return newBuffer;
}
