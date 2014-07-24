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
	: QGLWidget(parent),
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

	int timerInterval = 500 / _screenFrequency;
	_refreshTimer->start( timerInterval);
	//PHDEBUG << "Refresh rate set to " << _screenFrequency << "hz, timer restart every" << timerInterval << "ms";
	_dropTimer.start();
}

PhGraphicView::PhGraphicView(int width, int height, QWidget *parent)
	: PhGraphicView(parent)
{
	int ratio = this->windowHandle()->devicePixelRatio();
	this->setGeometry(0, 0, width / ratio, height / ratio);
}

PhGraphicView::~PhGraphicView()
{
	_refreshTimer->stop();
	TTF_Quit();
	SDL_Quit();
}

void PhGraphicView::resizeGL(int width, int height)
{
	if(height == 0)
		height = 1;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glLoadIdentity();
}

void PhGraphicView::setGraphicSettings(PhGraphicSettings *settings)
{
	_settings = settings;
}

void PhGraphicView::addInfo(QString info)
{
	_infos.append(info);
}

void PhGraphicView::onRefresh()
{
	if(this->refreshRate() > _maxRefreshRate)
		_maxRefreshRate = this->refreshRate();
	addInfo(QString("refresh: %1x%2, %3 / %4")
	        .arg(this->width())
	        .arg(this->height())
	        .arg(_maxRefreshRate)
	        .arg(this->refreshRate()));
	addInfo(QString("Update : %1 %2").arg(_maxUpdateDuration).arg(_lastUpdateDuration));
	addInfo(QString("drop: %1 %2").arg(_dropDetected).arg(_dropTimer.elapsed() / 1000));

	QTime t;
	t.start();
	updateGL();
	_lastUpdateDuration = t.elapsed();
	if(_lastUpdateDuration > _maxUpdateDuration)
		_maxUpdateDuration = _lastUpdateDuration;
	if(_lastUpdateDuration > 1500 / _screenFrequency) {
		_dropTimer.restart();
		_dropDetected++;
	}

}

void PhGraphicView::paintGL()
{
	//PHDEBUG << "PhGraphicView::paintGL" ;
	emit beforePaint(_screenFrequency);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QTime timer;
	timer.start();

	int ratio = this->windowHandle()->devicePixelRatio();
	emit paint(this->width() * ratio, this->height() * ratio);

	if(timer.elapsed() > _maxPaintDuration)
		_maxPaintDuration = timer.elapsed();
	addInfo(QString("draw: %1 %2").arg(_maxPaintDuration).arg(timer.elapsed()));
	if(_settings) {
		if(_settings->resetInfo()) {
			_dropDetected = 0;
			_maxRefreshRate = 0;
			_maxPaintDuration = 0;
			_maxUpdateDuration = 0;
		}
		if(_settings->displayInfo()) {
			_infoFont.setFontFile(_settings->infoFontFile());
			int y = 0;
			foreach(QString info, _infos) {
				PhGraphicText gInfo(&_infoFont, info, 0, y);
				gInfo.setSize(_infoFont.getNominalWidth(info) / 2, 50);
				gInfo.setZ(10);
				gInfo.setColor(Qt::red);
				gInfo.draw();
				y += gInfo.height();
			}
		}
	}
	// Once the informations have been displayed
	// clear it
	_infos.clear();

	_frameTickCounter.tick();
}
