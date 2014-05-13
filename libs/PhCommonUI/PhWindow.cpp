#include <QTimer>
#include <QAction>
#include <QGuiApplication>

#include "PhTools/PhDebug.h"

#include "PhWindow.h"

PhWindow::PhWindow(PhWindowSettings *settings) :
	QMainWindow(NULL),
	_settings(settings)
{
	// Set up a filter for catching event
	this->installEventFilter(this);

	if(_settings->fullScreen()) {
		PHDEBUG << "Going fullscreen...";
		QTimer::singleShot(1000, this, SLOT(showFullScreen()));
	}
	else {
		PHDEBUG << "Restoring geometry...";
		QMainWindow::restoreGeometry(_settings->windowGeometry());
	}

	connect(qApp, SIGNAL(applicationStateChanged(Qt::ApplicationState)), this, SLOT(onApplicationStateChange(Qt::ApplicationState)));
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

void PhWindow::onApplicationStateChange(Qt::ApplicationState state)
{
	switch(state) {
	case Qt::ApplicationActive:
		onApplicationActivate();
		break;
	case Qt::ApplicationInactive:
		onApplicationDeactivate();
		break;
	default:
		break;
	}
}

void PhWindow::toggleFullScreen()
{
	PHDEBUG;
	if(this->isFullScreen()) {
		PHDEBUG << "switch to normal";
		QMainWindow::showNormal();
	}
	else {
		PHDEBUG << "switch to fullscreen";
		QMainWindow::showFullScreen();
	}
}
