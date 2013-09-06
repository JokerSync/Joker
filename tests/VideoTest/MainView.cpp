#include "MainView.h"
#include "ui_MainView.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QFileInfo>
#include <QFileDialog>

MainView::MainView()
	: QMainWindow(0),
	  ui(new Ui::MainView)
{
	_clock = new PhClock;
	ui->setupUi(this);
	ui->_videoView->setClock(_clock);

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


	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFile()));
	connect(ui->_videoView, SIGNAL(positionChangedSignal(qint64)), this, SLOT(updateFrame(qint64)));

}


MainView::~MainView()
{
	delete ui;
}

bool MainView::openFile(QString fileName)
{
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists())
    {
		ui->_videoView->open(fileName);
		_clock->setRate(0.0);
		return true;
    }
	return false;
}


void MainView::onOpenFile()
{
	 QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
	 openFile(fileName); // TODO: show error in case of error
}

void MainView::updateFrame(qint64 position)
{
	PhFrame f = position*PhTimeCode::getFps(_clock->getTCType())/1000;
	qDebug() << "position:"<<position<<"frame"<<f;//_clock->frame();
	_clock->setFrame(f);
}



void MainView::pushPlayButton()
{
	if(_clock->rate() == 0)//If state = pause
		_clock->setRate(1.0);
	else //If state = play
		_clock->setRate(0.0);
}


void MainView::pushForwardButton()
{
	_clock->setRate(3.0);
}


void MainView::pushRewindButton()
{
	_clock->setRate(-3.0);
}


void MainView::pushBackButton()
{
	_clock->setRate(0);
	_clock->setFrame(0);
}


void MainView::pushNextFrameButton()
{
	PhFrame f = _clock->frame() + 1;
	_clock->setFrame(f);
}


void MainView::pushPreviousFrameButton()
{
	PhFrame f = _clock->frame() - 1;
	_clock->setFrame(f);
}


void MainView::useSliderCursor(int position)
{
	_clock->setFrame(position);
}


void MainView::selectRate(int index)
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

void MainView::backToBeginning()
{
	_clock->setRate(0.0);
	_clock->setFrame(ui->mediaController->getMediaLength());
}
