#include "PhVideoView.h"

#include <QFile>

PhVideoView::PhVideoView(QObject *parent)
	: QVideoWidget(),
	_player(this,QMediaPlayer::VideoSurface),
	_clock(NULL)
{
	_player.setVideoOutput(this);
}

bool PhVideoView::open(QString fileName)
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

void PhVideoView::setClock(PhClock *clock)
{
	_clock = clock;
	connect(_clock, SIGNAL(frameChanged()), this, SLOT(onFrameChanged()));
	connect(_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));
}

void PhVideoView::onRateChanged(PhRate rate)
{
	if(rate == 0)
		_player.pause();
	else
	{
		_player.play();
		_player.setPlaybackRate(rate);
	}

}

void PhVideoView::onFrameChanged()
{
	qDebug() << "PhVideoView::onFrameChanged() TODO";
}
