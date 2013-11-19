#include "VideoStripView.h"

VideoStripView::VideoStripView(QWidget *parent) :
	PhGraphicView(parent),
	_settings(NULL),
	_sony(NULL)
{
}

void VideoStripView::setSettings(QSettings *settings)
{
	_settings = settings;
	_strip.setSettings(settings);
}

void VideoStripView::setSony(PhSonyController *sony)
{
	_sony = sony;
}

bool VideoStripView::init()
{
	return _strip.init();
}

void VideoStripView::paint()
{
	if(_sony)
		_sony->checkVideoSync();

	float stripHeightRatio = 0.25f;
	if(_settings){
		stripHeightRatio = _settings->value("stripHeight", 0.25f).toFloat();
	}
	int videoHeight = this->height() * (1 - stripHeightRatio);

	if((_videoEngine.height() > 0) and (videoHeight > 0))
	{
		int videoWidth = videoHeight * _videoEngine.width() / _videoEngine.height();
		int videoX = (this->width() - videoWidth) / 2;
		_videoEngine.drawVideo(videoX, 0, videoWidth, videoHeight);
	}
	if(stripHeightRatio >= 0)
		_strip.draw(0, videoHeight, this->width(), this->height() - videoHeight);
}
