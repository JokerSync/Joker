#include <QCoreApplication>
#include <QTimer>
#include <QAction>

#include "PhTools/PhDebug.h"

#include "PhWindow.h"

PhWindow::PhWindow(PhWindowSettings *settings) :
	QMainWindow(NULL),
	_settings(settings)
{
	// Set up a filter for catching event
	qApp->installEventFilter(this);

	if(_settings->fullScreen()) {
		PHDEBUG << "Going fullscreen...";
		QTimer::singleShot(1000, this, SLOT(showFullScreen()));
	}
	else {
		PHDEBUG << "Restoring geometry...";
		QMainWindow::restoreGeometry(_settings->windowGeometry());
	}
}

bool PhWindow::eventFilter(QObject *sender, QEvent *event)
{
	Q_UNUSED(sender)
	switch(event->type()) {
	case QEvent::WindowStateChange:
		{
			_settings->setFullScreen(QMainWindow::isFullScreen());
			_settings->setWindowGeometry(QMainWindow::saveGeometry());
			if(this->fullScreenAction()) {
				this->fullScreenAction()->setCheckable(true);
				this->fullScreenAction()->setChecked(QMainWindow::isFullScreen());
			}
		}
		break;
	default:
		break;
	}

	return false;
}

void PhWindow::moveEvent(QMoveEvent *)
{
	_settings->setWindowGeometry(QMainWindow::saveGeometry());
}

void PhWindow::resizeEvent(QResizeEvent *)
{
	_settings->setWindowGeometry(QMainWindow::saveGeometry());
}

void PhWindow::toggleFullScreen()
{
	if(QMainWindow::isFullScreen())
		QMainWindow::showNormal();
	else
		QMainWindow::showFullScreen();
}
