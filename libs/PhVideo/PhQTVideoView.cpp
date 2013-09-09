#include "PhQTVideoView.h"

#include <QFile>

PhQTVideoView::PhQTVideoView(QObject *parent)
	: QVideoWidget(),
	_player(this,QMediaPlayer::VideoSurface)
{
	qDebug() << "Using QTVideo widget for video playback.";
	_player.setVideoOutput(this);

	connect(&_player, SIGNAL(positionChanged(qint64)), this, SIGNAL(positionChangedSignal(qint64)));
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
	//qint64 p = frame*1000/PhTimeCode::getFps(tcType);
	//_player.setPosition(p);
}
