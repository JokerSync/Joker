#include <QFile>

#include "PhQTVideoView.h"
#include "PhTools/PhDebug.h"

PhQTVideoView::PhQTVideoView(QObject *parent)
	: QVideoWidget(),
	_player(this,QMediaPlayer::VideoSurface)
{
	qDebug() << "Using QTVideo widget for video playback.";
	_player.setVideoOutput(this);
}

bool PhQTVideoView::open(QString fileName)
{
	if(QFile::exists(fileName))
	{
		_player.setMedia(QUrl::fromLocalFile(fileName));
		_player.pause();
		return true;
	}
	else
	{
		qDebug() << "File does not exist: " << fileName;
		return false;
	}
}

void PhQTVideoView::setClock(PhClock *clock)
{
	PhVideoObject::setClock(clock);
	connect(_clock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
	connect(_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));
	connect(_clock, SIGNAL(tcTypeChanged(PhTimeCodeType)), this, SLOT(onTCTypeChanged(PhTimeCodeType)));

	emit onTCTypeChanged(clock->timeCodeType());
}

void PhQTVideoView::onRateChanged(PhRate rate)
{
	if(rate == 0)
		_player.pause();
	else
		_player.play();

	_player.setPlaybackRate(rate);

}

void PhQTVideoView::onFrameChanged(PhFrame frame,PhTimeCodeType tcType)
{
	if(_player.playbackRate() == 0)
		_player.setPosition(_clock->milliSecond());
}

void PhQTVideoView::onTCTypeChanged(PhTimeCodeType tcType)
{
	_player.setNotifyInterval(1000/PhTimeCode::getFps(tcType));
}

void PhQTVideoView::checkVideoPosition()
{
	_clock->setMillisecond(_player.position());
}

