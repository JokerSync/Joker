/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QtGlobal>
#include <SDL2/SDL.h>
#if defined(Q_OS_MAC)
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL_ttf.h>
#endif
#include <QtGui>
#include "PhGraphicText.h"

#include "PhTools/PhDebug.h"
#include "PhGraphicView.h"

PhGraphicView::PhGraphicView( QWidget *parent)
	//: QQuickWidget(parent),
	: QQuickView(),
	_settings(NULL),
	_dropDetected(0),
	_maxRefreshRate(0),
	_maxPaintDuration(0),
	_lastUpdateDuration(0),
	_maxUpdateDuration(0)
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		PHDEBUG << "SDL error:" << SDL_GetError();
	if (TTF_Init() != 0)
		PHDEBUG << "TTF error:" << TTF_GetError();

	_refreshTimer = new QTimer(this);
	connect(_refreshTimer, SIGNAL(timeout()), this, SLOT(onRefresh()));

	//set the screen frequency to the most common value (60hz);
	_screenFrequency = 60;
	QScreen *screen = QGuiApplication::primaryScreen();
	if (screen)
		_screenFrequency = screen->refreshRate();
	else
		PHDEBUG << "Unable to get the screen";

	int timerInterval = 1000 / _screenFrequency;
	_refreshTimer->start( timerInterval);
	//PHDEBUG << "Refresh rate set to " << _screenFrequency << "hz, timer restart every" << timerInterval << "ms";
	_dropTimer.start();
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

	emit beforePaint(_screenFrequency);
	update();

	// Note: the update duration may not be interesting at all, since it only *schedules*
	// the paint request, but does not repaint immediately
	#warning /// @todo measure time between two updates instead of update time
	// the time between two updates may be more significant.
	_lastUpdateDuration = t.elapsed();
	if(_lastUpdateDuration > _maxUpdateDuration)
		_maxUpdateDuration = _lastUpdateDuration;
	if(_lastUpdateDuration > 1500 / _screenFrequency) {
		_dropTimer.restart();
		_dropDetected++;
	}

	_frameTickCounter.tick();
}




