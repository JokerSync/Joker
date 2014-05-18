/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QtGlobal>
#include <QApplication>
#include <QMouseEvent>

#include "VideoStripView.h"

VideoStripView::VideoStripView(QWidget *parent) :
	PhGraphicView(parent),
	_settings(NULL),
	_videoEngine(false),
	_sony(NULL),
	_titleText(_strip.getHUDFont(), ""),
	_tcText(_strip.getHUDFont(), "00:00:00:00"),
	_nextTCText(_strip.getHUDFont(), "00:00:00:00"),
	_noVideoSyncError(_strip.getHUDFont(), "No video sync"),
	_currentPeopleName(_strip.getHUDFont(), "")
{
	qApp->installEventFilter(this);
}

void VideoStripView::setSettings(JokerSettings *settings)
{
	_settings = settings;
	_strip.setSettings(settings);
	this->setGraphicSettings(settings);
}

void VideoStripView::setSony(PhSonyController *sony)
{
	_sony = sony;
	if(_sony) {
		connect(_sony, SIGNAL(videoSync()), this, SLOT(onVideoSync()));
		_lastVideoSyncElapsed.start();
	}
}

bool VideoStripView::init()
{
	_titleBackgroundRect.setColor(QColor(0, 0, 128));
	_titleText.setColor(QColor(255, 255, 255));
	_tcText.setColor(Qt::green);
	_nextTCText.setColor(Qt::red);
	_noVideoSyncError.setColor(QColor(0, 0, 0));
	_currentPeopleName.setColor(QColor(128, 128, 128));

	connect(this, SIGNAL(beforePaint(PhTimeScale)), _strip.clock(), SLOT(tick(PhTimeScale)));
	_logo.setFilename(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/phonations.png");
	_logo.init();

	return _strip.init();
}

void VideoStripView::paint()
{
	PHDBG(1) << _strip.clock()->time() - (_sony ? _sony->clock()->time() : 0);

	QList<PhPeople*> selectedPeoples;
	foreach(QString name, _settings->selectedPeopleNameList()) {
		PhPeople *people = _strip.doc()->peopleByName(name);
		if(people)
			selectedPeoples.append(people);
	}

	int y = 0;
	QString title = _strip.doc()->title();
	if(_strip.doc()->episode().length() > 0)
		title += " #" + _strip.doc()->episode();

	if(_settings->displayTitle() && (title.length() > 0)) {
		int titleHeight = this->height() / 40;
		_titleBackgroundRect.setRect(0, y, this->width(), titleHeight);
		int titleWidth = title.length() * titleHeight / 2;
		int titleX = (this->width() - titleWidth) / 2;
		_titleText.setRect(titleX, y, titleWidth, titleHeight);
		y += titleHeight;
		_titleText.setContent(title);
		_titleText.setZ(5);
		_titleBackgroundRect.setZ(5);
	}

	float stripHeightRatio = 0.25f;
	if(_settings) {
		if(_settings->hideStrip())
			stripHeightRatio = 0;
		else
			stripHeightRatio = _settings->stripHeight();
	}

	int stripHeight = (this->height() - y) * stripHeightRatio;
	int videoHeight = this->height() - y - stripHeight;

	int tcOffset = 0;
	if(_settings->displayNextTC())
		tcOffset = _nextTCText.height();
	if(_settings->displayTitle())
		tcOffset += _titleText.height();

	_strip.draw(0, y + videoHeight, this->width(), stripHeight, tcOffset, selectedPeoples);
	foreach(QString info, _strip.infos()) {
		this->addInfo(info);
	}

	// The strip must be the first drawn object, otherwise it masks previous drawings.
	if(_settings->displayTitle() && (title.length() > 0)) {
		_titleBackgroundRect.draw();
		_titleText.draw();
	}

	int tcWidth = 200;

	if((videoHeight > 0)) {
		if(_videoEngine.height() > 0) {
			int videoWidth;
			if(_forceRatio169)
				videoWidth = videoHeight * 16 / 9;
			else
				videoWidth = videoHeight * _videoEngine.width() / _videoEngine.height();

			int blackStripHeight = 0; // Height of the upper black strip when video is too large
			int realVideoHeight = videoHeight;
			if(videoWidth > this->width()) {
				videoWidth = this->width();
				if(_forceRatio169)
					realVideoHeight = videoWidth  * 9 / 16;
				else
					realVideoHeight = videoWidth  * _videoEngine.height() / _videoEngine.width();
			}
			blackStripHeight = (this->height() - stripHeight - realVideoHeight) / 2;

			int videoX = (this->width() - videoWidth) / 2;
			_videoEngine.drawVideo(videoX, y + blackStripHeight, videoWidth, realVideoHeight);

			// adjust tc size
			if(videoX > tcWidth)
				tcWidth = videoX;
			else if( this->width() < 2 * tcWidth)
				tcWidth = this->width() / 2;
		}
		else if(_settings->displayLogo()) {
			// The logo file is 500px in native format
			int logoHeight = _logo.originalSize().height();
			int logoWidth = _logo.originalSize().width();
			if(videoHeight < logoHeight) {
				logoHeight = videoHeight;
				logoWidth = _logo.originalSize().width() * logoHeight / _logo.originalSize().height();
			}
			if(this->width() < logoWidth) {
				logoWidth = this->width();
				logoHeight = _logo.originalSize().height() * logoWidth / _logo.originalSize().width();
			}
			_logo.setRect((this->width() - logoHeight) / 2, (videoHeight - logoHeight) / 2, logoHeight, logoHeight);
			_logo.draw();

		}
	}

	PhClock *clock = _videoEngine.clock();
	long delay = (int)(24 * _settings->screenDelay() * clock->rate());
	PhTime clockTime = clock->time() + delay;
	int tcHeight = tcWidth / 5;

	if(_settings->displayTC()) {
		_tcText.setRect(0, y, tcWidth, tcHeight);
		_tcText.setContent(PhTimeCode::stringFromTime(clockTime, clock->timeCodeType()));
		_tcText.draw();
	}

	if(_settings->displayNextTC()) {
		PhStripText *nextText = NULL;

		_nextTCText.setRect(this->width() - tcWidth, y, tcWidth, tcHeight);
		y += tcHeight;

		/// The next time code will be the next element of the people from the list.
		if(selectedPeoples.count()) {
			nextText = _strip.doc()->nextText(selectedPeoples, clockTime);
			if(nextText == NULL)
				nextText = _strip.doc()->nextText(selectedPeoples, 0);

			int peopleHeight = this->height() / 30;
			foreach(PhPeople* people, selectedPeoples) {
				int peopleNameWidth = people->name().length() * peopleHeight / 2;
				_currentPeopleName.setRect(10, y, peopleNameWidth, peopleHeight);
				_currentPeopleName.setContent(people->name());
				_currentPeopleName.draw();
				y += peopleHeight;
			}
		}
		else {
			nextText = _strip.doc()->nextText(clockTime);
			if(nextText == NULL)
				nextText = _strip.doc()->nextText(0);
		}

		if(nextText != NULL) {
			_nextTCText.setContent(PhTimeCode::stringFromTime(nextText->timeIn(), clock->timeCodeType()));
			_nextTCText.draw();
		}
	}

	PhStripLoop * currentLoop = _strip.doc()->previousLoop(clockTime);
	if(currentLoop) {
		int loopNumber = currentLoop->number();
		PhGraphicText gCurrentLoop(_strip.getHUDFont(), QString::number(loopNumber));
		int loopHeight = 60;
		int loopWidth = _strip.getHUDFont()->getNominalWidth(QString::number(loopNumber)) * ((float) loopHeight / _strip.getHUDFont()->getHeight());
		gCurrentLoop.setRect(10, this->height() - stripHeight - loopHeight, loopWidth, loopHeight);
		gCurrentLoop.setColor(Qt::blue);
		gCurrentLoop.draw();
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

bool VideoStripView::eventFilter(QObject *, QEvent *event)
{
	switch(event->type()) {
	case QEvent::MouseMove:
		{
			QMouseEvent * mouseEvent = (QMouseEvent*)event;
			float stripHeight = this->height() * _settings->stripHeight();
			if((mouseEvent->pos().y() > (this->height() - stripHeight) * 0.95)
			   && (mouseEvent->pos().y() < (this->height() - stripHeight) * 1.05)) {
				QApplication::setOverrideCursor(Qt::SizeVerCursor);
				if(mouseEvent->buttons() & Qt::LeftButton)
					_settings->setStripHeight(1.0 - ((float) mouseEvent->pos().y() /(float) this->height()));
			}
			else
				QApplication::setOverrideCursor(Qt::ArrowCursor);
		}
	default:
		break;
	}
	return false;
}

void VideoStripView::onVideoSync()
{
	_lastVideoSyncElapsed.restart();
}
