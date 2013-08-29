#include "PhQTVideoView.h"

#include <QFile>

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
		qDebug() << "File does not exist: "<< fileName;
		return false;
	}
}

void PhQTVideoView::setClock(PhClock *clock)
{
	PhVideoObject::setClock(clock);
	connect(_clock, SIGNAL(frameChanged()), this, SLOT(onFrameChanged()));
	connect(_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));
}

void PhQTVideoView::onRateChanged(PhRate rate)
{
	if(rate == 0)
		_player.pause();
	else
	{
		_player.play();
		_player.setPlaybackRate(rate);
	}

}

void PhQTVideoView::onFrameChanged()
{
	qDebug() << "PhQTVideoView::onFrameChanged() TODO";
}
