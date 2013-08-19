#include "PhMediaControllerView.h"
#include "ui_PhMediaControllerView.h"

PhMediaControllerView::PhMediaControllerView(PhClock *clock, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PhMediaControllerView)
{
	ui->setupUi(this);
	_clock = new PhClock;
	_clock = clock;

	//Buttons Init

	ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	connect(ui->_playButton, SIGNAL(clicked()), this, SLOT(pushPlayButton()));

	ui->_fastForwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
	connect(ui->_fastForwardButton, SIGNAL(clicked()), this, SLOT(pushForwardButton()));

	ui->_fastRewindButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
	connect(ui->_fastRewindButton, SIGNAL(clicked()), this, SLOT(pushRewindButton()));

	ui->_backButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
	connect(ui->_backButton, SIGNAL(clicked()), this, SLOT(pushBackButton()));

	ui->_nextFrameButton->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
	connect(ui->_nextFrameButton, SIGNAL(clicked()), this, SLOT(pushNextFrameButton()));

	ui->_previousFrameButton->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
	connect(ui->_previousFrameButton, SIGNAL(clicked()), this, SLOT(pushPreviousFrameButton()));

	//Label Init

	ui->_rateLabel->setStyleSheet("font:24pt");

	//ui->_rateLabel->setText("rate: "+QString::number(_clock->getRate()));
	ui->_rateLabel->setText("rate: 0");

	//ui->_timecodeLabel->setText(PhTimeCode::stringFromFrame(_clock->getFrame(),PhTimeCodeType25));
	ui->_timecodeLabel->setText("00:00:00:00");

	//Combobox Init

	ui->_rateSelectionBox->addItem("rate: 0");
	ui->_rateSelectionBox->addItem("rate: 1");
	ui->_rateSelectionBox->addItem("rate: 4");
	ui->_rateSelectionBox->addItem("rate:-4");
	//connect(ui->_rateSelectionBox, SIGNAL(activated(int)), this, SLOT(selectRate()));

	connect(ui->_rateSelectionBox, SIGNAL(activated(int)), this, SLOT(selectRate()));

	//this->setLayout(ui->_vLayout);
	this->resize(320, 80);

	//Connections SIGNALS/SLOTS
	connect(_clock, SIGNAL(rateChanged()), this, SLOT(updateRateLabel()));
	connect(_clock, SIGNAL(frameChanged()), this, SLOT(updateFrameLabel()));
}


void PhMediaControllerView::pushPlayButton()
{
	if(_clock->getRate() == 0)//If state = pause
	{
		_clock->setRate(1);
		ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

	}
	else //If state = play
	{
		_clock->setRate(0);
		ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	}
	playButtonSignal();
}


void PhMediaControllerView::updateFrame()
{
	_clock->tick();
}


void PhMediaControllerView::pushForwardButton()
{
	_clock->setRate(4);
	ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
	forwardButtonSignal();
}


void PhMediaControllerView::pushRewindButton()
{
	_clock->setRate(-4);
	ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
	rewindButtonSignal();
}

void PhMediaControllerView::pushBackButton()
{
	_clock->setRate(0);
	_clock->setFrame(0);
	backButtonSignal();
}

void PhMediaControllerView::pushNextFrameButton()
{
	PhFrame f = _clock->getFrame();
	_clock->setFrame(f+1);
}

void PhMediaControllerView::pushPreviousFrameButton()
{
	PhFrame f = _clock->getFrame();
	_clock->setFrame(f-1);
}


void PhMediaControllerView::updateRateLabel()
{
	ui->_rateLabel->setText("rate: "+QString::number(_clock->getRate()));
}


void PhMediaControllerView::updateFrameLabel()
{
	ui->_timecodeLabel->setText(PhTimeCode::stringFromFrame(_clock->getFrame(),PhTimeCodeType25));
}


void PhMediaControllerView::selectRate()
{
	ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

	if(ui->_rateSelectionBox->currentIndex() == 0)
	{
		_clock->setRate(0);
		ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	}

	if(ui->_rateSelectionBox->currentIndex() == 1)
	_clock->setRate(1);

	if(ui->_rateSelectionBox->currentIndex() == 2)
	_clock->setRate(4);

	if(ui->_rateSelectionBox->currentIndex() == 3)
	_clock->setRate(-4);

}





PhMediaControllerView::~PhMediaControllerView()
{
	delete ui;
}
