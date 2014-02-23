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

	if(_settings->fullScreen())
		QTimer::singleShot(1000, this, SLOT(showFullScreen()));
	else
		QMainWindow::restoreGeometry(_settings->windowGeometry());
}

bool PhWindow::eventFilter(QObject *sender, QEvent *event)
{
	switch(event->type()) {
	case QEvent::WindowStateChange:
		{
			_settings->setFullScreen(QMainWindow::isFullScreen());
			if(this->fullScreenAction())
				this->fullScreenAction()->setChecked(QMainWindow::isFullScreen());
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
