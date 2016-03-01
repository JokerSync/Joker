#include "SecondScreenWindow.h"

#include "PhGraphic/PhGraphicSolidRect.h"

SecondScreenWindow::SecondScreenWindow(PhVideoEngine *videoEngine, PhGraphicView *shareWidget, JokerSettings *settings)
	: PhGraphicView(NULL, shareWidget), _videoEngine(videoEngine), _jokerSettings(settings)
{
	this->installEventFilter(this);

	this->restoreGeometry(_jokerSettings->videoSecondScreenGeometry());

	this->connect(this, &PhGraphicView::paint, this, &SecondScreenWindow::onPaint);
}

void SecondScreenWindow::closeEvent(QCloseEvent *)
{
	_jokerSettings->setVideoSecondScreenGeometry(this->saveGeometry());
	emit closing();
}

bool SecondScreenWindow::eventFilter(QObject *, QEvent *event)
{
	switch (event->type()) {
	case QEvent::MouseButtonDblClick:
		if(this->isFullScreen())
			SecondScreenWindow::showNormal();
		else
			SecondScreenWindow::showFullScreen();
		return true;
	default:
		return false;
	}
}

void SecondScreenWindow::onPaint(int width, int height)
{
	PhTime delay = (PhTime)(24 * _jokerSettings->videoSecondScreenDelay() * _videoEngine->clock()->rate());
	_videoEngine->drawVideo(0, 0, width, height, delay);
}
