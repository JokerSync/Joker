#include "PhFFMpegVideoView.h"
#include <QVBoxLayout>

using namespace QtAV;

PhFFMpegVideoView::PhFFMpegVideoView(QWidget *parent) :
	QWidget(parent), _player(this)
{
	mpRenderer = VideoRendererFactory::create(VideoRendererId_Widget);

	_player.setRenderer(mpRenderer);

	QVBoxLayout * layout = new QVBoxLayout(this);
	layout->addWidget(mpRenderer->widget());
}

bool PhFFMpegVideoView::open(QString fileName)
{
	return _player.play(fileName);
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
	_player.setSpeed(rate);
}

void PhFFMpegVideoView::checkVideoPosition()
{

}
