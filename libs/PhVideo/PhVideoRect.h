#ifndef PHVIDEORECT_H
#define PHVIDEORECT_H

#include "PhGraphic/PhGraphicTexturedRect.h"

#include "PhVideoBuffer.h"

class PhVideoRect : public PhGraphicTexturedRect
{
public:
	PhVideoRect();

	void update(PhVideoBuffer *buffer);

	PhFrame currentFrame() const;

	void discard();
private:
	PhFrame _currentFrame;
};

#endif // PHVIDEORECT_H
