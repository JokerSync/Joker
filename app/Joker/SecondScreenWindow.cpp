#include <QTimer>
#include <QCloseEvent>

#include "PhTools/PhDebug.h"

#include "SecondScreenWindow.h"

SecondScreenWindow::SecondScreenWindow(PhVideoEngine *videoEngine, JokerSettings *settings)
	: PhGraphicView(NULL), _videoEngine(videoEngine), _jokerSettings(settings)
{
	this->installEventFilter(this);

	// FIXME
	//this->restoreGeometry(_jokerSettings->videoSecondScreenGeometry());

	if(_jokerSettings->videoSecondScreenFullscreen()) {
		PHDEBUG << "Going fullscreen...";
		QTimer::singleShot(1000, this, SLOT(showFullScreen()));
	}

	this->connect(this, &PhGraphicView::paint, this, &SecondScreenWindow::onPaint);
}

void SecondScreenWindow::closeEvent(QCloseEvent *event)
{
	PHDBG(1) << event->spontaneous();
// FIXME
//	if(!this->isFullScreen())
//		_jokerSettings->setVideoSecondScreenGeometry(this->saveGeometry());
//	_jokerSettings->setVideoSecondScreenFullscreen(this->isFullScreen());

	this->disconnect(this, &PhGraphicView::paint, this, &SecondScreenWindow::onPaint);

	emit closing(event->spontaneous());
}

bool SecondScreenWindow::eventFilter(QObject *, QEvent *event)
{
	switch (event->type()) {
	case QEvent::MouseButtonDblClick:
// FIXME
//		if(this->isFullScreen())
//			SecondScreenWindow::showNormal();
//		else
			SecondScreenWindow::showFullScreen();
		return true;
	default:
		return false;
	}
}

void SecondScreenWindow::onPaint(int width, int height)
{
	// FIXME
	//PhTime delay = (PhTime)(24 * _jokerSettings->videoSecondScreenDelay() * _videoEngine->clock()->rate());
	//_videoEngine->drawVideo(0, 0, width, height, delay);
}
