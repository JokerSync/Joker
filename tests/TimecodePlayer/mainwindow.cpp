#include "mainwindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	_clock = new PhClock;
	PhTimeCodeType timecodeType = PhTimeCodeType25;
	lengthFile = 7500;
	firstFrame = _clock->frame();
	ui->mediaController->setMediaLength(lengthFile);
	ui->mediaController->setTCType(timecodeType);
	ui->mediaController->setFirstFrame(firstFrame);
	PhFrame fps = PhTimeCode::getFps(timecodeType);

	connect(ui->mediaController, SIGNAL(playButtonSignal()), this, SLOT(pushPlayButton()));

	connect(ui->mediaController, SIGNAL(forwardButtonSignal()), this, SLOT(pushForwardButton()));

	connect(ui->mediaController, SIGNAL(rewindButtonSignal()), this, SLOT(pushRewindButton()));

	connect(ui->mediaController, SIGNAL(backButtonSignal()), this, SLOT(pushBackButton()));

	connect(ui->mediaController, SIGNAL(nextFrameButtonSignal()), this, SLOT(pushNextFrameButton()));

	connect(ui->mediaController, SIGNAL(previousFrameButtonSignal()), this, SLOT(pushPreviousFrameButton()));

	connect(ui->mediaController, SIGNAL(useSliderCursorSignal(int)), this, SLOT(useSliderCursor(int)));


	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
	_timer->start(1000/fps);
}


MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::pushPlayButton()
{
	if(_clock->rate() == 0)//If state = pause
		_clock->setRate(1);
	else //If state = play
		_clock->setRate(0);

}


void MainWindow::pushForwardButton()
{
	_clock->setRate(4);
}


void MainWindow::pushRewindButton()
{
	_clock->setRate(-4);
}


void MainWindow::pushBackButton()
{
	_clock->setRate(0);
	_clock->setFrame(0);
}


void MainWindow::pushNextFrameButton()
{
	PhFrame f = _clock->frame() + 1;
	_clock->setFrame(f);
}


void MainWindow::pushPreviousFrameButton()
{
	PhFrame f = _clock->frame() - 1;
	_clock->setFrame(f);
}


void MainWindow::useSliderCursor(int position)
{
	_clock->setFrame(position);
}

