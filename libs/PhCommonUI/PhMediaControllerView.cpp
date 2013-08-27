#include "PhMediaControllerView.h"
#include "ui_PhMediaControllerView.h"

PhMediaControllerView::PhMediaControllerView(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PhMediaControllerView),
	_tcType(PhTimeCodeType25),
	_clock(NULL),
	_firstFrame(0),
	_mediaLength(0)
{
	ui->setupUi(this);

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

	connect(ui->_slider, SIGNAL(sliderMoved(int)), this, SLOT(useSliderCursor(int)));

	//Combobox Init

	ui->_rateSelectionBox->addItem("23.98 fps");
	ui->_rateSelectionBox->addItem("24 fps");
	ui->_rateSelectionBox->addItem("25 fps");
	ui->_rateSelectionBox->addItem("29.97 fps");

	connect(ui->_rateSelectionBox, SIGNAL(activated(int)), this, SLOT(selectRate()));

	//Connections SIGNALS/SLOTS



}

PhMediaControllerView::~PhMediaControllerView()
{
	delete ui;
}

void PhMediaControllerView::setTCType(PhTimeCodeType tcType)
{
	_tcType = tcType;
}

PhTimeCodeType PhMediaControllerView::getTCType() const
{
	return _tcType;
}

void PhMediaControllerView::setClock(PhClock *clock)
{
	_clock = clock;
	connect(_clock, SIGNAL(rateChanged()), this, SLOT(updateRateLabel()));
	connect(_clock, SIGNAL(frameChanged()), this, SLOT(updateFrameLabel()));
}

PhClock *PhMediaControllerView::getClock() const
{
	return _clock;
}


void PhMediaControllerView::setFirstFrame(PhFrame firstFrame)
{
	_firstFrame = firstFrame;
	ui->_slider->setMinimum(firstFrame);
}

PhFrame PhMediaControllerView::getFirstFrame() const
{
	return _firstFrame;
}

void PhMediaControllerView::setMediaLength(qint64 mediaLength)
{
	_mediaLength = mediaLength;
	ui->_slider->setMaximum(_firstFrame + mediaLength);
}



void PhMediaControllerView::pushPlayButton()
{
	if(_clock->getRate() == 0)//If state = pause
		_clock->setRate(1);
	else //If state = play
		_clock->setRate(0);

	playButtonSignal();
}


void PhMediaControllerView::updateFrame()
{
	_clock->tick();
}


void PhMediaControllerView::pushForwardButton()
{
	_clock->setRate(4);
	forwardButtonSignal();
}


void PhMediaControllerView::pushRewindButton()
{
	_clock->setRate(-4);
	rewindButtonSignal();
}

void PhMediaControllerView::pushBackButton()
{
	_clock->setRate(0);
	_clock->setFrame(_firstFrame);

	backButtonSignal();
}

void PhMediaControllerView::pushNextFrameButton()
{
	PhFrame f = _clock->getFrame() + 1;
	_clock->setFrame(f);
	nextFrameButtonSignal();
}

void PhMediaControllerView::pushPreviousFrameButton()
{
	PhFrame f = _clock->getFrame() - 1;
	_clock->setFrame(f);
	previousFrameButtonSignal();
}

void PhMediaControllerView::useSliderCursor(int position)
{
	int t = position*_mediaLength/100 + _firstFrame;
	_clock->setFrame(t);
	useSliderCursorSignal();
}


void PhMediaControllerView::updateRateLabel()
{
	ui->_rateLabel->setText("rate: "+QString::number(_clock->getRate()));
	if(_clock->getRate() != 0)
		ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

	else
		ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}


void PhMediaControllerView::updateFrameLabel()
{
	ui->_timecodeLabel->setText(PhTimeCode::stringFromFrame(_clock->getFrame(),_tcType));
	updateSliderPosition();

}

void PhMediaControllerView::updateSliderPosition()
{
	int t = _clock->getFrame();
	ui->_slider->setSliderPosition(t);

	if(t >= _firstFrame + _mediaLength)
	{
		_clock->setRate(0);
		_clock->setFrame(_firstFrame);
	}
}


void PhMediaControllerView::selectRate()
{
	switch(ui->_rateSelectionBox->currentIndex())
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

}






