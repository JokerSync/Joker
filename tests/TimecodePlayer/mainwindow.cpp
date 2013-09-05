#include "mainwindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	_clock = new PhClock;


	_clock->setTCType(PhTimeCodeType25);
	ui->mediaController->setMediaLength(7500);
	ui->mediaController->setTCType(_clock->getTCType());
	ui->mediaController->setFirstFrame(_clock->frame());

	connect(ui->mediaController, SIGNAL(playButtonSignal()), this, SLOT(pushPlayButton()));
	connect(ui->mediaController, SIGNAL(forwardButtonSignal()), this, SLOT(pushForwardButton()));
	connect(ui->mediaController, SIGNAL(rewindButtonSignal()), this, SLOT(pushRewindButton()));
	connect(ui->mediaController, SIGNAL(backButtonSignal()), this, SLOT(pushBackButton()));
	connect(ui->mediaController, SIGNAL(nextFrameButtonSignal()), this, SLOT(pushNextFrameButton()));
	connect(ui->mediaController, SIGNAL(previousFrameButtonSignal()), this, SLOT(pushPreviousFrameButton()));
	connect(ui->mediaController, SIGNAL(useSliderCursorSignal(int)), this, SLOT(useSliderCursor(int)));
	connect(ui->mediaController, SIGNAL(useComboBoxSignal(int)), this, SLOT(selectRate(int)));

	connect(_clock, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), ui->mediaController, SLOT(onFrameChanged(PhFrame, PhTimeCodeType)));
	connect(_clock, SIGNAL(rateChanged(PhRate)), ui->mediaController, SLOT(onRateChanged(PhRate)));

	connect(ui->mediaController, SIGNAL(endOfMediaSignal()), this, SLOT(backToBeginning()));


	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
	_timer->start(1000/PhTimeCode::getFps(_clock->getTCType()));
}


MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::updateFrame()
{
	_clock->tick(PhTimeCode::getFps(_clock->getTCType()));
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
	_clock->setRate(3);
}


void MainWindow::pushRewindButton()
{
	_clock->setRate(-3);
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


void MainWindow::selectRate(int index)
{
	switch(index)
	{
	case 0:
		_clock->setTCType(PhTimeCodeType2398);
		break;
	case 1:
		_clock->setTCType(PhTimeCodeType24);
		break;
	case 2:
		_clock->setTCType(PhTimeCodeType25);
		break;
	case 3:
		_clock->setTCType(PhTimeCodeType2997);
		break;
	}
}

void MainWindow::backToBeginning()
{
	_clock->setRate(0);
	_clock->setFrame(ui->mediaController->getMediaLength());
}

