#include "PhVideoObject.h"

PhVideoObject::PhVideoObject() : _clock(PhTimeCodeType25)
{
}

void PhVideoObject::setFrameStamp(PhFrame frame)
{
	_frameStamp = frame;
}

