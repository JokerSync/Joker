#include "PhVideoObject.h"

PhVideoObject::PhVideoObject() : _clock(PhTimeCodeType25), _frameStamp(0)
{
}

void PhVideoObject::setFrameStamp(PhFrame frame)
{
	_frameStamp = frame;
}

