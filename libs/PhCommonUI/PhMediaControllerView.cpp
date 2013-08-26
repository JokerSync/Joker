#include "PhMediaControllerView.h"
#include "ui_PhMediaControllerView.h"

PhMediaControllerView::PhMediaControllerView(PhClock *clock, qint64 lengthFile, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PhMediaControllerView)
{
	ui->setupUi(this);
	_clock = new PhClock;
	_clock = clock;


	_lengthFile = lengthFile;
	_firstFrame = 0;
	_timecodeType = PhTimeCodeType25;
	_framePerSecond = PhTimeCode::getFps(_timecodeType);
	_fileProgress = 0;

	_clock->setFrame(_firstFrame);

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



	//Label Init

	ui->_rateLabel->setStyleSheet("font:24pt");
	ui->_rateLabel->setText("rate: "+QString::number(_clock->getRate()));
	ui->_timecodeLabel->setText(PhTimeCode::stringFromFrame(_clock->getFrame(),PhTimeCodeType25));

	//Combobox Init

	ui->_rateSelectionBox->addItem("23.98 fps");
	ui->_rateSelectionBox->addItem("24 fps");
	ui->_rateSelectionBox->addItem("25 fps");
	ui->_rateSelectionBox->addItem("29.97 fps");


	connect(ui->_rateSelectionBox, SIGNAL(activated(int)), this, SLOT(selectRate()));

	//this->setLayout(ui->_vLayout);

	this->setFixedHeight(100);
	this->resize(320, 80);

	//Connections SIGNALS/SLOTS
	connect(_clock, SIGNAL(rateChanged()), this, SLOT(updateRateLabel()));
	connect(_clock, SIGNAL(frameChanged()), this, SLOT(updateFrameLabel()));


}

int PhMediaControllerView::get_framePerSecond() const
{
	return _framePerSecond;
}

void PhMediaControllerView::set_fileInfo(qint64 lengthFile)
{
	_lengthFile = lengthFile;
	_firstFrame = 0;
	_timecodeType = PhTimeCodeType25;
	_framePerSecond = 25;
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
	rewindButtonSignal();
	_clock->setRate(-4);
	ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
}

void PhMediaControllerView::pushBackButton()
{
	_clock->setRate(0);
	_clock->setFrame(_firstFrame);
	ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
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
	int t = position*_lengthFile/100 + _firstFrame;
	_clock->setFrame(t);
	useSliderCursorSignal();
}


void PhMediaControllerView::updateRateLabel()
{
	ui->_rateLabel->setText("rate: "+QString::number(_clock->getRate()));
}


void PhMediaControllerView::updateFrameLabel()
{
	ui->_timecodeLabel->setText(PhTimeCode::stringFromFrame(_clock->getFrame(),_timecodeType));
	updateSliderPosition();

}

void PhMediaControllerView::updateSliderPosition()
{
	int t = _clock->getFrame();
	_fileProgress = (t - _firstFrame)*100/_lengthFile;
	//qDebug() << t <<"  "<< _fileProgress <<"  "<<_lengthFile;
	ui->_slider->setSliderPosition(_fileProgress);

	if(_fileProgress == 100)
	{
		_clock->setRate(0);
		_clock->setFrame(0);
	}
}


void PhMediaControllerView::selectRate()
{
	switch(ui->_rateSelectionBox->currentIndex())
	{
	case 0:
		_timecodeType = PhTimeCodeType2398;
		break;
	case 1:
		_timecodeType =	PhTimeCodeType24;
		break;
	case 2:
		_timecodeType = PhTimeCodeType25;
		break;
	case 3:
		_timecodeType = PhTimeCodeType2997;
		break;

	}
	_framePerSecond = PhTimeCode::getFps(_timecodeType);


}





PhMediaControllerView::~PhMediaControllerView()
{
	delete ui;
}
