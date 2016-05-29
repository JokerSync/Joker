/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "PhGraphicText.h"

#include "PhGraphicView.h"

PhGraphicView::PhGraphicView(QWidget *parent)
	: QQuickWidget(parent),
	//: QQuickView(),
	//: QQmlApplicationEngine(),
	_settings(NULL),
	_dropDetected(0),
	_maxRefreshRate(0),
	_maxPaintDuration(0),
	_lastUpdateDuration(0),
	_maxUpdateDuration(0),
	_previousNsecsElapsed(0)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		PHDEBUG << "SDL error:" << SDL_GetError();
	if (TTF_Init() != 0)
		PHDEBUG << "TTF error:" << TTF_GetError();

	_refreshTimer = new QTimer(this);
	connect(_refreshTimer, SIGNAL(timeout()), this, SLOT(onRefresh()));

	//set the screen frequency to the most common value (60hz);
	_screenFrequency = 60.0;
	QScreen *screen = QGuiApplication::primaryScreen();
	if (screen)
		_screenFrequency = screen->refreshRate();
	else
		PHDEBUG << "Unable to get the screen";

	int timerInterval = static_cast<int>(1000.0 / _screenFrequency);
	_refreshTimer->start( timerInterval);
	//PHDEBUG << "Refresh rate set to " << _screenFrequency << "hz, timer restart every" << timerInterval << "ms";
	_dropTimer.start();

	_timer.start();
}

PhGraphicView::PhGraphicView(int width, int height, QWidget *parent)
	: PhGraphicView(parent)
{
	//int ratio = this->windowHandle()->devicePixelRatio();
	//this->setGeometry(0, 0, width / ratio, height / ratio);
}

PhGraphicView::~PhGraphicView()
{
	_refreshTimer->stop();
	TTF_Quit();
	SDL_Quit();
}

void PhGraphicView::setGraphicSettings(PhGraphicSettings *settings)
{
	_settings = settings;
}

int PhGraphicView::maxRefreshRate()
{
	return _maxRefreshRate;
}

int PhGraphicView::maxUpdateDuration()
{
	return _maxUpdateDuration;
}

int PhGraphicView::lastUpdateDuration()
{
	return _lastUpdateDuration;
}

int PhGraphicView::dropDetected()
{
	return _dropDetected;
}

int PhGraphicView::secondsSinceLastDrop()
{
	return _dropTimer.elapsed() / 1000;
}

void PhGraphicView::onRefresh()
{
	// Update the clock time according to the time elapsed since the last paint event
	// In the particular case of V-sync enabled and no dropped frames, this is equivalent
	// to using the screen refresh rate to compute the elapsed time.
	// If V-sync is not enabled, using the screen refresh rate is plain wrong
	// (the refreshTimer period would be ok though).
	// If frames are dropped, only an actual timer can be correct.
	// Millisecond precision is not enough (60 Hz is 16.6 ms), so we use nanoseconds.
	qint64 nsecsElapsed = _timer.nsecsElapsed();
	double elapsedSeconds = static_cast<double>(nsecsElapsed - _previousNsecsElapsed) / 1000000000.0f;
	_previousNsecsElapsed = nsecsElapsed;

	if(_settings) {
		if(_settings->resetInfo()) {
			_dropDetected = 0;
			_maxRefreshRate = 0;
			_maxPaintDuration = 0;
			_maxUpdateDuration = 0;
		}
	}

	if(this->refreshRate() > _maxRefreshRate)
		_maxRefreshRate = this->refreshRate();

	QTime t;
	t.start();

	emit beforePaint(static_cast<PhTime> (24000.0 * elapsedSeconds));
	update();

	// Note: the update duration may not be interesting at all, since it only *schedules*
	// the paint request, but does not repaint immediately
	#warning /// @todo measure time between two updates instead of update time
	// the time between two updates may be more significant.
	_lastUpdateDuration = t.elapsed();
	if(_lastUpdateDuration > _maxUpdateDuration)
		_maxUpdateDuration = _lastUpdateDuration;
	if(_lastUpdateDuration > static_cast<int>(1500.0 / _screenFrequency)) {
		_dropTimer.restart();
		_dropDetected++;
	}

	_frameTickCounter.tick();
}
