/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QKeyEvent>

#include "PhTools/PhDebug.h"

#include "PhMediaPanel.h"
#include "ui_PhMediaPanel.h"

PhMediaPanel::PhMediaPanel(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::PhMediaPanel),
	_clock(NULL),
	_timeIn(0),
	_length(0),
	_playing(false)
{
	ui->setupUi(this);

	//Buttons Init

	connect(ui->_playButton, &QPushButton::clicked, this, &PhMediaPanel::onPlayPause);

	connect(ui->_fastForwardButton, &QPushButton::clicked, this, &PhMediaPanel::onFastForward);

	connect(ui->_fastRewindButton, &QPushButton::clicked, this, &PhMediaPanel::onRewind);

	connect(ui->_backButton, &QPushButton::clicked, this, &PhMediaPanel::onBack);

	connect(ui->_nextFrameButton, &QPushButton::clicked, this, &PhMediaPanel::onNextFrame);

	connect(ui->_previousFrameButton, &QPushButton::clicked, this, &PhMediaPanel::onPreviousFrame);

	connect(ui->_slider, &QSlider::sliderMoved, this, &PhMediaPanel::onSliderChanged);

	connect(ui->timeCodeTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onTCTypeComboChanged()));

	ui->_playButton->setDefault(true);

	installEventFilter(this);
}


PhMediaPanel::~PhMediaPanel()
{
	delete ui;
}

PhTimeCodeType PhMediaPanel::timeCodeType() const
{
	switch(ui->timeCodeTypeCombo->currentIndex()) {
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

void PhMediaPanel::setSliderEnable(bool enabled)
{
	ui->_slider->setEnabled(enabled);
}

void PhMediaPanel::setDropdownEnable(bool enabled)
{
	ui->timeCodeTypeCombo->setEnabled(enabled);
}

PhTime PhMediaPanel::timeIn() const
{
	return _timeIn;
}

void PhMediaPanel::setTimeIn(PhTime timeIn)
{
	_timeIn = timeIn;
	updateSlider();
}

PhTime PhMediaPanel::length()
{
	return _length;
}

void PhMediaPanel::setLength(PhTime length)
{
	_length = length;
	updateSlider();
}

bool PhMediaPanel::isPlaying()
{
	if(_clock)
		return (_clock->rate() != 0);
	else
		return _playing;
}

void PhMediaPanel::setTimeCodeType(PhTimeCodeType tcType)
{
	onTimeCodeTypeChanged(tcType);
}


void PhMediaPanel::setClock(PhClock *clock)
{
	_clock = clock;
	if(_clock) {
		ui->_timecodeLabel->setText(PhTimeCode::stringFromTime(_clock->time(), this->timeCodeType()));
		connect(_clock, &PhClock::timeChanged, this, &PhMediaPanel::onTimeChanged);
		connect(_clock, &PhClock::rateChanged, this, &PhMediaPanel::onRateChanged);
	}
}

void PhMediaPanel::onRateChanged(PhRate rate)
{
	ui->_rateLabel->setText("x"+QString::number(rate));
	updatePlayingState();
}

void PhMediaPanel::onTimeCodeTypeChanged(PhTimeCodeType tcType)
{
	switch(tcType) {
	case PhTimeCodeType2398:
		ui->timeCodeTypeCombo->setCurrentIndex(0);
		break;
	case PhTimeCodeType24:
		ui->timeCodeTypeCombo->setCurrentIndex(1);
		break;
	case PhTimeCodeType25:
		ui->timeCodeTypeCombo->setCurrentIndex(2);
		break;
	case PhTimeCodeType2997:
		ui->timeCodeTypeCombo->setCurrentIndex(3);
		break;
	case PhTimeCodeType30:
		ui->timeCodeTypeCombo->setCurrentIndex(4);
		break;
	}
}

bool PhMediaPanel::eventFilter(QObject *, QEvent *event)
{
	if(event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = (QKeyEvent*)event;
		if(keyEvent->key() == Qt::Key_Space) {
			onPlayPause();
			return true;
		}
	}

	return false;
}

void PhMediaPanel::onPlayPause()
{
	if(_clock) {
		if(_clock->rate())
			_clock->setRate(0);
		else
			_clock->setRate(1);
	}
	else
		_playing = !_playing;
	updatePlayingState();

	if(_playing)
		emit playClicked();
	else
		emit pauseClicked();
}

void PhMediaPanel::onFastForward()
{
	if(_clock)
		_clock->setRate(3);
	emit fastForwardClicked();
}

void PhMediaPanel::onRewind()
{
	if(_clock)
		_clock->setRate(-3);
	emit rewindClicked();
}

void PhMediaPanel::onBack()
{
	if(_clock)
		_clock->setTime(_timeIn);
	emit backClicked();
}

void PhMediaPanel::onNextFrame()
{
	if(_clock) {
		_clock->setRate(0);
		_clock->setTime(_clock->time() + PhTimeCode::timePerFrame(this->timeCodeType()));
	}
	emit nextFrameClicked();
}

void PhMediaPanel::onPreviousFrame()
{
	if(_clock) {
		_clock->setRate(0);
		_clock->setTime(_clock->time() - PhTimeCode::timePerFrame(this->timeCodeType()));
	}
	emit previousFrameClicked();
}

void PhMediaPanel::onSliderChanged(int position)
{
	PhTime time = position * PhTimeCode::timePerFrame(this->timeCodeType());
	if(_clock)
		_clock->setTime(time);
	emit sliderMoved(time);
}

void PhMediaPanel::updateSlider()
{
	PhTimeCodeType tcType = this->timeCodeType();
	PhFrame frameIn = _timeIn / PhTimeCode::timePerFrame(tcType);
	PhFrame frameOut = (_timeIn + _length) / PhTimeCode::timePerFrame(tcType);
	ui->_slider->setMinimum(frameIn);
	ui->_slider->setMaximum(frameOut);
}

void PhMediaPanel::onTCTypeComboChanged()
{
	emit timeCodeTypeChanged(timeCodeType());
}

void PhMediaPanel::updatePlayingState()
{
	if(_clock)
		_playing = (_clock->rate() != 0);
	PHDEBUG << _playing;
	if(_playing)
		ui->_playButton->setIcon(QIcon(":pause"));
	else
		ui->_playButton->setIcon(QIcon(":play"));
}

void PhMediaPanel::onTimeChanged(PhTime time)
{
	PhTimeCodeType tcType = this->timeCodeType();
	ui->_timecodeLabel->setText(PhTimeCode::stringFromTime(time, tcType));
	ui->_slider->setSliderPosition(time / PhTimeCode::timePerFrame(tcType));
}

