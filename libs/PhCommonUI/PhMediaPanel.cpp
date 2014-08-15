/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhMediaPanel.h"
#include "ui_PhMediaPanel.h"

PhMediaPanel::PhMediaPanel(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PhMediaPanel),
	_clock(NULL),
	_timeIn(0),
	_length(0)
{
	ui->setupUi(this);

	//Buttons Init

	ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
	connect(ui->_playButton, SIGNAL(clicked()), this, SLOT(onPlayPause()));

	ui->_fastForwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
	connect(ui->_fastForwardButton, SIGNAL(clicked()), this, SLOT(onFastForward()));

	ui->_fastRewindButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
	connect(ui->_fastRewindButton, SIGNAL(clicked()), this, SLOT(onRewind()));

	ui->_backButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
	connect(ui->_backButton, SIGNAL(clicked()), this, SLOT(onBack()));

	ui->_nextFrameButton->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
	connect(ui->_nextFrameButton, SIGNAL(clicked()), this, SLOT(onNextFrame()));

	ui->_previousFrameButton->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
	connect(ui->_previousFrameButton, SIGNAL(clicked()), this, SLOT(onPreviousFrame()));

	connect(ui->_slider, SIGNAL(sliderMoved(int)), this, SLOT(onSliderChanged(int)));

	//Combobox Init

	ui->_rateSelectionBox->addItem("23.98 fps");
	ui->_rateSelectionBox->addItem("24 fps");
	ui->_rateSelectionBox->addItem("25 fps");
	ui->_rateSelectionBox->addItem("29.97 fps");

	connect(ui->_rateSelectionBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTCTypeComboChanged()));

	ui->_playButton->setDefault(true);
}


PhMediaPanel::~PhMediaPanel()
{
	delete ui;
}


void PhMediaPanel::setTCType(PhTimeCodeType tcType)
{
	onTimeCodeTypeChanged(tcType);

	if(_clock)
		_clock->setTimeCodeType(tcType);
}


PhTimeCodeType PhMediaPanel::timeCodeType() const
{
	switch(ui->_rateSelectionBox->currentIndex()) {
	case 0:
		return PhTimeCodeType2398;
	case 1:
		return PhTimeCodeType24;
	case 3:
		return PhTimeCodeType2997;
	case 4:
		return PhTimeCodeType30;
	default:
		return PhTimeCodeType25;
	}
}

void PhMediaPanel::setFirstFrame(PhFrame firstFrame)
{
	setTimeIn(firstFrame * PhTimeCode::timePerFrame(this->timeCodeType()));
}

void PhMediaPanel::setTimeIn(PhTime timeIn)
{
	_timeIn = timeIn;
	ui->_slider->setMinimum(_timeIn / 24000);
	ui->_slider->setMaximum((_timeIn + _length) / 24000);
}

PhFrame PhMediaPanel::getFirstFrame() const
{
	return _timeIn / PhTimeCode::timePerFrame(this->timeCodeType());
}

void PhMediaPanel::setMediaLength(PhFrame length)
{
	setLength(length * PhTimeCode::timePerFrame(this->timeCodeType()));
}

void PhMediaPanel::setLength(PhTime length)
{
	_length = length;
	ui->_slider->setMaximum((_timeIn + length) / 24000);
}

PhFrame PhMediaPanel::getMediaLength()
{
	return _length / PhTimeCode::timePerFrame(this->timeCodeType());
}

PhTime PhMediaPanel::length()
{
	return _length;
}

void PhMediaPanel::setSliderEnable(bool isEnabled)
{
	ui->_slider->setEnabled(isEnabled);
}

void PhMediaPanel::setClock(PhClock *clock)
{
	_clock = clock;
	if(_clock) {
		setTCType(_clock->timeCodeType());
		ui->_timecodeLabel->setText(PhTimeCode::stringFromFrame(_clock->frame(), _clock->timeCodeType()));
		connect(_clock, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame, PhTimeCodeType)));
		connect(_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));
		connect(_clock, SIGNAL(tcTypeChanged(PhTimeCodeType)), this, SLOT(onTimeCodeTypeChanged(PhTimeCodeType)));
	}
}


void PhMediaPanel::onRateChanged(PhRate rate)
{
	ui->_rateLabel->setText("x"+QString::number(rate));
	if(rate != 0)
		ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
	else
		ui->_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
}

void PhMediaPanel::onTimeCodeTypeChanged(PhTimeCodeType tcType)
{
	switch(tcType) {
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
	case PhTimeCodeType30:
		ui->_rateSelectionBox->setCurrentIndex(4);
		break;
	}
}

void PhMediaPanel::onPlayPause()
{
	if(_clock) {
		if(_clock->rate())
			_clock->setRate(0);
		else
			_clock->setRate(1);
	}
	emit playPause();
}

void PhMediaPanel::onFastForward()
{
	if(_clock)
		_clock->setRate(3);
	emit fastForward();
}

void PhMediaPanel::onRewind()
{
	if(_clock)
		_clock->setRate(-3);
	emit rewind();
}

void PhMediaPanel::onBack()
{
	if(_clock)
		_clock->setTime(_timeIn);
	emit back();
}

void PhMediaPanel::onNextFrame()
{
	if(_clock) {
		_clock->setRate(0);
		_clock->setFrame(_clock->frame() + 1);
	}
	emit nextFrame();
}

void PhMediaPanel::onPreviousFrame()
{
	if(_clock) {
		_clock->setRate(0);
		_clock->setFrame(_clock->frame() - 1);
	}
	emit previousFrame();
}

void PhMediaPanel::onSliderChanged(int position)
{
	PhFrame frame = (PhFrame)position;
	if(_clock)
		_clock->setFrame(frame);
	emit goToFrame(frame, timeCodeType());
}

void PhMediaPanel::onTCTypeComboChanged()
{
	if(_clock)
		_clock->setTimeCodeType(timeCodeType());
	emit timeCodeTypeChanged(timeCodeType());
}

void PhMediaPanel::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	ui->_timecodeLabel->setText(PhTimeCode::stringFromFrame(frame, tcType));
	ui->_slider->setSliderPosition(frame);
}

