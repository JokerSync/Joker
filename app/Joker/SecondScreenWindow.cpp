#include <QTimer>

#include "PhTools/PhDebug.h"
#include "PhGraphic/PhGraphicSolidRect.h"

#include "SecondScreenWindow.h"

SecondScreenWindow::SecondScreenWindow(PhVideoEngine *videoEngine, PhGraphicView *shareWidget, JokerSettings *settings)
	: PhGraphicView(NULL, shareWidget), _videoEngine(videoEngine), _jokerSettings(settings)
{
	this->installEventFilter(this);

	this->restoreGeometry(_jokerSettings->videoSecondScreenGeometry());

	if(_jokerSettings->videoSecondScreenFullscreen()) {
		PHDEBUG << "Going fullscreen...";
		QTimer::singleShot(1000, this, SLOT(showFullScreen()));
	}

	this->connect(this, &PhGraphicView::paint, this, &SecondScreenWindow::onPaint);
}

void SecondScreenWindow::closeEvent(QCloseEvent *)
{
	if(!this->isFullScreen())
		_jokerSettings->setVideoSecondScreenGeometry(this->saveGeometry());
	_jokerSettings->setVideoSecondScreenFullscreen(this->isFullScreen());
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
