#include "SecondScreenWindow.h"

#include "PhGraphic/PhGraphicSolidRect.h"

SecondScreenWindow::SecondScreenWindow(PhVideoEngine *videoEngine, PhGraphicView *shareWidget, JokerSettings *settings)
	: PhGraphicView(NULL, shareWidget), _videoEngine(videoEngine), _jokerSettings(settings)
{
	this->connect(this, &PhGraphicView::paint, this, &SecondScreenWindow::onPaint);
}

void SecondScreenWindow::onPaint(int width, int height)
{
	PhTime delay = (PhTime)(24 * _jokerSettings->videoSecondScreenDelay() * _videoEngine->clock()->rate());
	_videoEngine->drawVideo(0, 0, width, height, delay);
}
