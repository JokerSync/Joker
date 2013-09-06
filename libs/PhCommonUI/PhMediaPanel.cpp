#include "PhMediaPanel.h"
#include "ui_PhMediaPanel.h"

PhMediaPanel::PhMediaPanel(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PhMediaPanel),
	_tcType(PhTimeCodeType25),
	_firstFrame(0),
	_mediaLength(0)
{
	ui->setupUi(this);

	//Buttons Init

	ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	connect(ui->_playButton, SIGNAL(clicked()), this, SIGNAL(playButtonSignal()));

	ui->_fastForwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
	connect(ui->_fastForwardButton, SIGNAL(clicked()), this, SIGNAL(forwardButtonSignal()));

	ui->_fastRewindButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
	connect(ui->_fastRewindButton, SIGNAL(clicked()), this, SIGNAL(rewindButtonSignal()));

	ui->_backButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
	connect(ui->_backButton, SIGNAL(clicked()), this, SIGNAL(backButtonSignal()));

	ui->_nextFrameButton->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
	connect(ui->_nextFrameButton, SIGNAL(clicked()), this, SIGNAL(nextFrameButtonSignal()));

	ui->_previousFrameButton->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
	connect(ui->_previousFrameButton, SIGNAL(clicked()), this, SIGNAL(previousFrameButtonSignal()));

	connect(ui->_slider, SIGNAL(sliderMoved(int)), this, SIGNAL(useSliderCursorSignal(int)));

	//Combobox Init

	ui->_rateSelectionBox->addItem("23.98 fps");
	ui->_rateSelectionBox->addItem("24 fps");
	ui->_rateSelectionBox->addItem("25 fps");
	ui->_rateSelectionBox->addItem("29.97 fps");

	connect(ui->_rateSelectionBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(useComboBoxSignal(int)));

	setFixedSize(600,100);
}


PhMediaPanel::~PhMediaPanel()
{
	delete ui;
}


void PhMediaPanel::setTCType(PhTimeCodeType tcType)
{
	_tcType = tcType;

	switch(tcType)
	{
	case PhTimeCodeType2398:
		ui->_rateSelectionBox->setCurrentIndex(0);
		break;
	case PhTimeCodeType24:
		ui->_rateSelectionBox->setCurrentIndex(1);
		break;
	case PhTimeCodeType25:
		ui->_rateSelectionBox->setCurrentIndex(2);
		break;
	case PhTimeCodeType2997:
		ui->_rateSelectionBox->setCurrentIndex(3);
		break;
	}
}


PhTimeCodeType PhMediaPanel::getTCType() const
{
	return _tcType;
}


void PhMediaPanel::setFirstFrame(PhFrame firstFrame)
{
	_firstFrame = firstFrame;
	ui->_slider->setMinimum(firstFrame);
	ui->_slider->setMaximum(_firstFrame + _mediaLength);
}


PhFrame PhMediaPanel::getFirstFrame() const
{
	return _firstFrame;
}


void PhMediaPanel::setMediaLength(PhFrame mediaLength)
{
	_mediaLength = mediaLength;
	ui->_slider->setMaximum(_firstFrame + mediaLength);
}


PhFrame PhMediaPanel::getMediaLength()
{
	return _mediaLength;
}


void PhMediaPanel::onRateChanged(PhRate rate)
{
	ui->_rateLabel->setText("x"+QString::number(rate));
	if(rate != 0)
		ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));

	else
		ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}


void PhMediaPanel::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	ui->_timecodeLabel->setText(PhTimeCode::stringFromFrame(frame,tcType));

	ui->_slider->setSliderPosition(frame);

	if(frame >= _firstFrame + _mediaLength)
	{
		emit endOfMediaSignal();
	}
}

