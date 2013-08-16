#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{

	_clock = new PhClock;
	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
	_timer->start(40);

	_mediaControllerView = new PhMediaControllerView(_clock);
	_mediaControllerView->show();

	//Connections SIGNALS/SLOTS
	connect(_clock, SIGNAL(rateChanged()), this, SLOT(updateRateLabel()));
	connect(_clock, SIGNAL(frameChanged()), this, SLOT(updateFrameLabel()));
}




void MainWindow::updateFrame()
{
	_clock->tick();
}

