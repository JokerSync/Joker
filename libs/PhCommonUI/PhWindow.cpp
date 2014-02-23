#include "PhWindow.h"

PhWindow::PhWindow(PhWindowSettings *settings, QWidget *parent) :
	QMainWindow(parent),
	_settings(settings)
{
	restoreGeometry(_settings->windowGeometry());
}

void PhWindow::moveEvent(QMoveEvent *)
{
	_settings->setWindowGeometry(saveGeometry());
}

void PhWindow::resizeEvent(QResizeEvent *)
{
	_settings->setWindowGeometry(saveGeometry());
}

