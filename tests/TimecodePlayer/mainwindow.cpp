#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
	PhFrame lenghtFile = 37500;
	PhTimeCodeType timecodeType = PhTimeCodeType25;
	_clock = new PhClock;
	_clock->setFrame(PhTimeCode::frameFromString("01:00:00:00",timecodeType));

	_mediaControllerView = new PhMediaControllerView(_clock, timecodeType, lenghtFile);
	_fps = new PhFrame;
	_fps = _mediaControllerView->get_framePerSecond();



	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
	_timer->start(1000/(*_fps));


	_mediaControllerView->show();





}




void MainWindow::updateFrame()
{
	_clock->tick();
}

