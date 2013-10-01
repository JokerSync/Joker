#include "PhFFMpegVideoView.h"
#include <QVBoxLayout>

using namespace QtAV;

PhFFMpegVideoView::PhFFMpegVideoView(QWidget *parent) :
	QWidget(parent), _player(this)
{
	qDebug() << "Using FFMpeg widget for video playback.";
	mpRenderer = VideoRendererFactory::create(VideoRendererId_Widget);

	_player.setRenderer(mpRenderer);

	QVBoxLayout * layout = new QVBoxLayout(this);
	layout->addWidget(mpRenderer->widget());

	connect(&_clock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
	connect(&_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));
}

bool PhFFMpegVideoView::open(QString fileName)
{
	if(_player.load(fileName))
	{
		_player.pause(true);
		return true;
	}
	return false;
}

void PhFFMpegVideoView::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	qint64 ms = (_clock.frame() - this->getFrameStamp()) * 1000 / PhTimeCode::getFps(tcType);
	//qDebug() << "frame" << _clock.frame() << "frameStamp" << this->getFrameStamp();
	PHDEBUG << "ms : " << ms << "\trate : " << _clock.rate();
	if(_clock.rate() == 0)
	{
		_player.seek(ms / 1000.0);
	}
}

void PhFFMpegVideoView::onRateChanged(PhRate rate)
{
	if(_player.isPaused())
	{
		_player.play();
		_player.setSpeed(rate);
	}
	else
		_player.pause(true);
}

void PhFFMpegVideoView::checkVideoPosition()
{

}
