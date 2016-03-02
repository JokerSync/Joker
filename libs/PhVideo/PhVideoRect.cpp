#include "PhVideoRect.h"


PhVideoRect::PhVideoRect() : _currentFrame(PHFRAMEMIN)
{
}

void PhVideoRect::update(PhVideoBuffer *buffer)
{
	_currentFrame = buffer->frame();
	createTextureFromBGRABuffer(buffer->rgb(), buffer->width(), buffer->height());
}

PhFrame PhVideoRect::currentFrame() const
{
	return _currentFrame;
}

void PhVideoRect::discard()
{
	_currentFrame = PHFRAMEMIN;
}
