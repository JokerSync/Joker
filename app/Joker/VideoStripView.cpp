#include "VideoStripView.h"
#include <QtGlobal>

VideoStripView::VideoStripView(QWidget *parent) :
	PhGraphicView(parent),
	_settings(NULL),
	_sony(NULL),
	_tcText(&_hudFont, "00:00:00:00"),
	_noVideoSyncError(&_hudFont, "No video sync")
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
#ifdef Q_OS_MAC
	_hudFont.setFontFile("/Library/Fonts/Arial.ttf");
#else
	_hudFont.setFontFile(qgetenv("windir") + "/Fonts/Arial.ttf");
#endif
	_tcText.setColor(QColor(128, 128, 128));
	_noVideoSyncError.setColor(QColor(0, 0, 0));

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
	int stripHeight = this->height() * stripHeightRatio;
	int videoHeight = this->height() - stripHeight;

	_strip.draw(0, videoHeight, this->width(), stripHeight);

	if((_videoEngine.height() > 0) and (videoHeight > 0))
	{
		int videoWidth = videoHeight * _videoEngine.width() / _videoEngine.height();
		int videoX = (this->width() - videoWidth) / 2;
		_videoEngine.drawVideo(videoX, 0, videoWidth, videoHeight);
	}

	int y = 0;
	if(_settings && _settings->value("displayTC", true).toBool())
	{
		PhClock *clock = _videoEngine.clock();
		long delay = (int)(_settings->value("delay", 0).toInt() * clock->rate()); // delay in ms
		PhFrame clockFrame = clock->frame() + delay * PhTimeCode::getFps(clock->timeCodeType()) / 1000;
		_tcText.setRect(0, 0, 200, 50);
		_tcText.setContent(PhTimeCode::stringFromFrame(clockFrame, clock->timeCodeType()));
		_tcText.draw();
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
