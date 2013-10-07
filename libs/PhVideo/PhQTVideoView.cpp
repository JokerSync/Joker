#include <QFile>

#include "PhQTVideoView.h"
#include "PhTools/PhDebug.h"

PhQTVideoView::PhQTVideoView(QObject *parent)
	: QVideoWidget() ,
	_player(this,QMediaPlayer::VideoSurface)
{
	qDebug() << "Using QTVideo widget for video playback.";
	_player.setVideoOutput(this);

	connect(&_clock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
	connect(&_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));
	connect(&_clock, SIGNAL(tcTypeChanged(PhTimeCodeType)), this, SLOT(onTCTypeChanged(PhTimeCodeType)));

	emit onTCTypeChanged(this->getClock()->timeCodeType());
}

bool PhQTVideoView::open(QString fileName)
{
	if(QFile::exists(fileName))
	{
		_player.setMedia(QUrl::fromLocalFile(fileName));
		_player.pause();
		_player.setNotifyInterval(10);
		return true;
	}
	else
	{
		qDebug() << "File does not exist: " << fileName;
		return false;
	}
}

void PhQTVideoView::onRateChanged(PhRate rate)
{
	if(rate == 0)
		_player.pause();
	else
		_player.play();

	_player.setPlaybackRate(rate);
}

void PhQTVideoView::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	qint64 ms = (_clock.frame() - this->frameStamp()) * 1000 / PhTimeCode::getFps(tcType);
	//qDebug() << "frame" << _clock.frame() << "frameStamp" << this->getFrameStamp();
	PHDEBUG << "ms : " << ms << "\trate : " << _clock.rate();
	if(_clock.rate() == 0)
	{
		_player.setPosition(ms);
	}
}

void PhQTVideoView::onTCTypeChanged(PhTimeCodeType tcType)
{
	_player.setNotifyInterval(1000/PhTimeCode::getFps(tcType));
}

void PhQTVideoView::checkVideoPosition()
{
	this->getClock()->setFrame(_player.position() * PhTimeCode::getFps(_clock.timeCodeType()) / 1000 + _frameStamp);
}
