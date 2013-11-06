#include "VideoStripView.h"

VideoStripView::VideoStripView(QWidget *parent) :
	PhGraphicView(parent),
	_settings(NULL)
{
}

void VideoStripView::setSettings(QSettings *settings)
{
	_settings = settings;
	_strip.setSettings(settings);
}

bool VideoStripView::init()
{
	return _strip.init();
}

void VideoStripView::paint()
{
	float stripHeightRatio = 0.25f;
	if(_settings){
		stripHeightRatio = _settings->value("stripHeight", 0.25f).toFloat();
		this->strip()->setPixelPerFrame(_settings->value("speed", 12).toInt());
		if(this->strip()->getFont()->getBoldness() != _settings->value("boldness", 0).toInt())
			this->strip()->getFont()->setBoldness(_settings->value("boldness", 0).toInt());
	}
	int videoHeight = this->height() * (1 - stripHeightRatio);
	if(videoHeight > 0)
	{
		int videoWidth = this->height();
		int videoX = (this->width() - videoWidth) / 2;
		_videoEngine.drawVideo(videoX, 0, videoWidth, videoHeight);
	}
	if(stripHeightRatio > 0)
		_strip.draw(0, videoHeight, this->width(), this->height() - videoHeight);
}
