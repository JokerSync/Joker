#include "SecondScreenWindow.h"

#include "PhGraphic/PhGraphicSolidRect.h"

SecondScreenWindow::SecondScreenWindow(PhVideoEngine *videoEngine, PhGraphicView *shareWidget)
	: PhGraphicView(NULL, shareWidget), _videoEngine(videoEngine)
{
	this->connect(this, &PhGraphicView::paint, this, &SecondScreenWindow::onPaint);
}

void SecondScreenWindow::onPaint(int width, int height)
{
	_videoEngine->drawVideo(0, 0, width, height);
}
