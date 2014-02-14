/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "VideoStripView.h"
#include <QtGlobal>

VideoStripView::VideoStripView(QWidget *parent) :
	PhGraphicView(parent),
	_sony(NULL),
	_titleText(_strip.getHUDFont(), ""),
	_tcText(_strip.getHUDFont(), "00:00:00:00"),
	_nextTCText(_strip.getHUDFont(), "00:00:00:00"),
	_noVideoSyncError(_strip.getHUDFont(), "No video sync"),
	_currentPeopleName(_strip.getHUDFont(), "")
{
	connect(_strip.doc(), SIGNAL(changed()), this, SLOT(onDocChanged()));
}

void VideoStripView::setSettings(QSettings *settings)
{
	PhGraphicView::setSettings(settings);
	_strip.setSettings(settings);
}

void VideoStripView::setSony(PhSonyController *sony)
{
	_sony = sony;
	if(_sony) {
		connect(_sony, SIGNAL(videoSync()), this, SLOT(onVideoSync()));
		_lastVideoSyncElapsed.start();
	}
}

QList<PhPeople *> *VideoStripView::getSelectedPeoples()
{
	return &_selectedPeoples;
}

bool VideoStripView::init()
{

	_titleBackgroundRect.setColor(QColor(0, 0, 128));
	_titleText.setColor(QColor(255, 255, 255));
	_tcText.setColor(QColor(128, 128, 128));
	_nextTCText.setColor(QColor(128, 128, 128));
	_noVideoSyncError.setColor(QColor(0, 0, 0));
	_currentPeopleName.setColor(QColor(128, 128, 128));

	return _strip.init();
}

void VideoStripView::paint()
{

	PHDBG(1) << _strip.clock()->time() - (_sony ? _sony->clock()->time() : 0);

	int y = 0;
	QString title = _strip.doc()->getTitle();
	if(_strip.doc()->getEpisode().length() > 0)
		title += " #" + _strip.doc()->getEpisode();

	if(_settings->value("displayTitle", true).toBool() && (title.length() > 0)) {
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

	if((_videoEngine.height() > 0)and (videoHeight > 0)) {
		int videoWidth = videoHeight * _videoEngine.width() / _videoEngine.height();
		int blackStripHeight = 0; // Height of the upper black strip when video is too large
		int realVideoHeight = videoHeight;
		if(videoWidth > this->width()) {
			videoWidth = this->width();
			realVideoHeight = videoWidth  * _videoEngine.height() / _videoEngine.width();
			blackStripHeight = (this->height() - stripHeight - videoHeight) / 2;
		}
		int videoX = (this->width() - videoWidth) / 2;
		_videoEngine.drawVideo(videoX, y + blackStripHeight, videoWidth, realVideoHeight);

		// adjust tc size
		if(videoX > tcWidth)
			tcWidth = videoX;
		else if( this->width() < 2 * tcWidth)
			tcWidth = this->width() / 2;
	}

	PhClock *clock = _videoEngine.clock();
	long delay = (int)(_settings->value("delay", 0).toInt() * clock->rate()); // delay in ms
	PhFrame clockFrame = clock->frame() + delay * PhTimeCode::getFps(clock->timeCodeType()) / 1000;
	int tcHeight = tcWidth / 5;

	if(_settings->value("displayTC", true).toBool()) {
		_tcText.setRect(0, y, tcWidth, tcHeight);
		_tcText.setContent(PhTimeCode::stringFromFrame(clockFrame, clock->timeCodeType()));
		_tcText.draw();
	}

	if(_settings->value("displayNextTC", true).toBool()) {
		PhStripText *nextText = NULL;

		_nextTCText.setRect(this->width() - tcWidth, y, tcWidth, tcHeight);
		y += tcHeight;

		if(_selectedPeoples.count()) {
			_strip.setSelectedPeople(&_selectedPeoples);
			nextText = _strip.doc()->getNextText(clockFrame, _selectedPeoples);
			if(nextText == NULL)
				nextText = _strip.doc()->getNextText(0, _selectedPeoples);

			int peopleHeight = this->height() / 30;
			foreach (PhPeople* people, _selectedPeoples) {
				int peopleNameWidth = people->getName().length() * peopleHeight / 2;
				_currentPeopleName.setRect(10, y, peopleNameWidth, peopleHeight);
				_currentPeopleName.setContent(people->getName());
				_currentPeopleName.draw();
				y += peopleHeight;
			}
		}
		else{
			_strip.setSelectedPeople(NULL);
			nextText = _strip.doc()->getNextText(clockFrame);
			if(nextText == NULL)
				nextText = _strip.doc()->getNextText(0);
		}

		if(nextText != NULL) {
			_nextTCText.setContent(PhTimeCode::stringFromFrame(nextText->getTimeIn(), clock->timeCodeType()));
			_nextTCText.draw();
		}
	}

	_noVideoSyncError.setRect(this->width() / 2 - 100, this->height() / 2 - 25, 200, 50);
	if(_lastVideoSyncElapsed.elapsed() > 1000) {
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

void VideoStripView::onDocChanged()
{
	_selectedPeoples.clear();
}
