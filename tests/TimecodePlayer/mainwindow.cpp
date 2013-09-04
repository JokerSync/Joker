#include "mainwindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	_clock = new PhClock;
	_tcType = PhTimeCodeType25;
	_lengthFile = 7500;
	_firstFrame = _clock->frame();
	ui->mediaController->setMediaLength(_lengthFile);
	ui->mediaController->setTCType(_tcType);
	ui->mediaController->setFirstFrame(_firstFrame);
	_frequence = PhTimeCode::getFps(_tcType);

	connect(ui->mediaController, SIGNAL(playButtonSignal()), this, SLOT(pushPlayButton()));
	connect(ui->mediaController, SIGNAL(forwardButtonSignal()), this, SLOT(pushForwardButton()));
	connect(ui->mediaController, SIGNAL(rewindButtonSignal()), this, SLOT(pushRewindButton()));
	connect(ui->mediaController, SIGNAL(backButtonSignal()), this, SLOT(pushBackButton()));
	connect(ui->mediaController, SIGNAL(nextFrameButtonSignal()), this, SLOT(pushNextFrameButton()));
	connect(ui->mediaController, SIGNAL(previousFrameButtonSignal()), this, SLOT(pushPreviousFrameButton()));
	connect(ui->mediaController, SIGNAL(useSliderCursorSignal(int)), this, SLOT(useSliderCursor(int)));
	connect(ui->mediaController, SIGNAL(useComboBoxSignal(int)), this, SLOT(selectRate(int)));

	connect(_clock, SIGNAL(frameChanged(PhFrame)), ui->mediaController, SLOT(onFrameChanged(PhFrame)));
	connect(_clock, SIGNAL(rateChanged(PhRate)), ui->mediaController, SLOT(onRateChanged(PhRate)));

	connect(ui->mediaController, SIGNAL(endOfMediaSignal()), this, SLOT(backToBeginning()));


	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
	_timer->start(1000/_frequence);

	resize(600,100);
}


MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::updateFrame()
{
	_clock->tick(_frequence);
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


void MainWindow::selectRate(int index)
{
	switch(index)
	{
	case 0:
		_tcType = PhTimeCodeType2398;
		break;
	case 1:
		_tcType =	PhTimeCodeType24;
		break;
	case 2:
		_tcType = PhTimeCodeType25;
		break;
	case 3:
		_tcType = PhTimeCodeType2997;
		break;
	}
	_frequence = PhTimeCode::getFps(_tcType);
	qDebug() << "frequence" << _frequence;
}

void MainWindow::backToBeginning()
{
	_clock->setRate(0);
	_clock->setFrame(_lengthFile);
}

