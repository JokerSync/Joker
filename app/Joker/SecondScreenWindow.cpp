#include "SecondScreenWindow.h"

#include "PhGraphic/PhGraphicSolidRect.h"

SecondScreenWindow::SecondScreenWindow(PhVideoEngine *videoEngine, PhGraphicView *shareWidget, JokerSettings *settings)
	: PhGraphicView(NULL, shareWidget), _videoEngine(videoEngine), _jokerSettings(settings)
{
	this->connect(this, &PhGraphicView::paint, this, &SecondScreenWindow::onPaint);
}

void SecondScreenWindow::onPaint(int width, int height)
{
	_videoEngine->drawVideo(0, 0, width, height, _jokerSettings->videoSecondScreenOffset());
}
