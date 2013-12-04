#include "VideoStripView.h"

VideoStripView::VideoStripView(QWidget *parent) :
	PhGraphicView(parent),
	_settings(NULL),
	_sony(NULL),
	_titleText(_strip.getHUDFont(), ""),
	_tcText(_strip.getHUDFont(), "00:00:00:00"),
	_nextTCText(_strip.getHUDFont(), "00:00:00:00"),
	_noVideoSyncError(_strip.getHUDFont(), "No video sync"),
	_currentPeople(NULL)
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

void VideoStripView::setPeople(PhPeople *people)
{
	_currentPeople = people;
}

bool VideoStripView::init()
{
	_titleBackgroundRect.setColor(QColor(0, 0, 128));
	_titleText.setColor(QColor(255, 255, 255));
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

	int y = 0;
	QString title = _strip.doc()->getTitle();
	if(_settings->value("displayTitle", true).toBool() && (title.length() > 0))
	{
		int titleHeight = this->height() / 40;
		_titleBackgroundRect.setRect(0, y, this->width(), titleHeight);
		int titleWidth = title.length() * titleHeight / 2;
		int titleX = (this->width() - titleWidth) / 2;
		_titleText.setRect(titleX, y, titleWidth, titleHeight);
		y += titleHeight;

		_titleBackgroundRect.draw();
		_titleText.setContent(title);
		_titleText.draw();
	}

	float stripHeightRatio = 0.25f;
	if(_settings)
		stripHeightRatio = _settings->value("stripHeight", 0.25f).toFloat();

	int stripHeight = (this->height() - y) * stripHeightRatio;
	int videoHeight = this->height() - y - stripHeight;

	_strip.draw(0, y + videoHeight, this->width(), stripHeight);

	int tcWidth = 200;

	if((_videoEngine.height() > 0) and (videoHeight > 0))
	{
		int videoWidth = videoHeight * _videoEngine.width() / _videoEngine.height();
		int videoX = (this->width() - videoWidth) / 2;
		_videoEngine.drawVideo(videoX, y, videoWidth, videoHeight);

		// adjust tc position
		tcWidth = videoX;
	}

	PhClock *clock = _videoEngine.clock();
	long delay = (int)(_settings->value("delay", 0).toInt() * clock->rate()); // delay in ms
	PhFrame clockFrame = clock->frame() + delay * PhTimeCode::getFps(clock->timeCodeType()) / 1000;
	int tcHeight = tcWidth / 5;

	if(_settings->value("displayTC", true).toBool())
	{
		_tcText.setRect(0, y, tcWidth, tcHeight);
		_tcText.setContent(PhTimeCode::stringFromFrame(clockFrame, clock->timeCodeType()));
		_tcText.draw();
	}

	if(_settings->value("displayNextTC", true).toBool())
	{
		PhStripText *nextText = NULL;

		if(_currentPeople)
		{
			nextText = _strip.doc()->getNextText(clockFrame, _currentPeople);
			if(nextText == NULL)
				nextText = _strip.doc()->getNextText(0, _currentPeople);
		}
		else
		{
			nextText = _strip.doc()->getNextText(clockFrame);
			if(nextText == NULL)
				nextText = _strip.doc()->getNextText(0);
		}

		_nextTCText.setRect(this->width() - tcWidth, y, tcWidth, tcHeight);
		if(nextText != NULL)
		{
			_nextTCText.setContent(PhTimeCode::stringFromFrame(nextText->getTimeIn(), clock->timeCodeType()));
			_nextTCText.draw();
		}
	}

	_noVideoSyncError.setRect(this->width() / 2 - 100, this->height() / 2 - 25, 200, 50);
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
