#include "VideoStripView.h"

VideoStripView::VideoStripView(QWidget *parent) :
	PhGraphicView(parent),
	_settings(NULL),
	_sony(NULL),
	_tcText(_strip.getHUDFont(), "00:00:00:00"),
	_nextTCText(_strip.getHUDFont(), "00:00:00:00"),
	_noVideoSyncError(_strip.getHUDFont(), "No video sync")
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
	if(_sony)
	{
		connect(_sony, SIGNAL(videoSync()), this, SLOT(onVideoSync()));
		_lastVideoSyncElapsed.start();
	}
}

bool VideoStripView::init()
{
	_tcText.setColor(QColor(128, 128, 128));
	_nextTCText.setColor(QColor(128, 128, 128));
	_noVideoSyncError.setColor(QColor(0, 0, 0));

	return _strip.init();
}

void VideoStripView::paint()
{
	PHDBG(1);

	if(_sony)
		_sony->checkVideoSync();

	float stripHeightRatio = 0.25f;
	if(_settings)
		stripHeightRatio = _settings->value("stripHeight", 0.25f).toFloat();

	int stripHeight = this->height() * stripHeightRatio;
	int videoHeight = this->height() - stripHeight;

	_strip.draw(0, videoHeight, this->width(), stripHeight);

	int tcWidth = 200;

	if((_videoEngine.height() > 0) and (videoHeight > 0))
	{
		int videoWidth = videoHeight * _videoEngine.width() / _videoEngine.height();
		int videoX = (this->width() - videoWidth) / 2;
		_videoEngine.drawVideo(videoX, 0, videoWidth, videoHeight);

		// adjust tc position
		tcWidth = videoX;
	}

	if(_settings && _settings->value("displayTC", true).toBool())
	{
		PhClock *clock = _videoEngine.clock();
		long delay = (int)(_settings->value("delay", 0).toInt() * clock->rate()); // delay in ms
		PhFrame clockFrame = clock->frame() + delay * PhTimeCode::getFps(clock->timeCodeType()) / 1000;
		int tcHeight = tcWidth / 5;
		_tcText.setRect(0, 0, tcWidth, tcHeight);
		_tcText.setContent(PhTimeCode::stringFromFrame(clockFrame, clock->timeCodeType()));
		_tcText.draw();

		PhFrame nextTextFrame = _strip.doc()->getNextTextFrame(clockFrame);
		if(nextTextFrame == PHFRAMEMAX)
			nextTextFrame = _strip.doc()->getNextTextFrame(0);

		_nextTCText.setRect(this->width() - tcWidth, 0, tcWidth, tcHeight);
		if(nextTextFrame < PHFRAMEMAX)
		{
			_nextTCText.setContent(PhTimeCode::stringFromFrame(nextTextFrame, clock->timeCodeType()));
			_nextTCText.draw();
		}
	}

	_noVideoSyncError.setRect(0, 50, 200, 50);
	if(_lastVideoSyncElapsed.elapsed() > 1000)
	{
		int red = (_lastVideoSyncElapsed.elapsed() - 1000) / 4;
		if (red > 255)
			red = 255;
		_noVideoSyncError.setColor(QColor(red, 0, 0));
		_noVideoSyncError.draw();
	}
	else
		_noVideoSyncError.setColor(QColor(0, 0, 0));
}

void VideoStripView::onVideoSync()
{
	_lastVideoSyncElapsed.restart();
}
