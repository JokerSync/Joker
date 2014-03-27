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
	  _settings(NULL)
{
	if (SDL_Init(SDL_INIT_VIDEO) == 0)
		PHDEBUG << "init SDL Ok.";
	else
		PHDEBUG << "SDL error:" << SDL_GetError();
	if (TTF_Init() == 0)
		PHDEBUG << "init TTF Ok.";
	else
		PHDEBUG << "TTF error:" << TTF_GetError();

	t_Timer = new QTimer(this);
	connect(t_Timer, SIGNAL(timeout()), this, SLOT(onRefresh()));

	//set the screen frequency to the most common value (60hz);
	_screenFrequency = 60;
	QScreen *screen = QGuiApplication::primaryScreen();
	if (screen)
		_screenFrequency = screen->refreshRate();
	else
		PHDEBUG << "Unable to get the screen";

	int timerInterval = 500 / _screenFrequency;
	t_Timer->start( timerInterval);
	PHDEBUG << "Refresh rate set to " << _screenFrequency << "hz, timer restart every" << timerInterval << "ms";
}

PhGraphicView::~PhGraphicView()
{
	t_Timer->stop();
	TTF_Quit();
	SDL_Quit();
}

void PhGraphicView::initializeGL()
{
	PHDEBUG;
	init();
}

void PhGraphicView::resizeGL(int width, int height)
{
	resize(width, height);
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

void PhGraphicView::setSettings(PhGraphicSettings *settings)
{
	_settings = settings;
	_infoFont.setFontFile(_settings->infoFontFile());
}

void PhGraphicView::addInfo(QString info)
{
	_infos.append(info);
}

void PhGraphicView::paintGL()
{
	//PHDEBUG << "PhGraphicView::paintGL" ;
	emit beforePaint(_screenFrequency);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);
	_infos.clear();
	addInfo(QString("refresh: %1").arg(this->refreshRate()));
	paint();
	if(_settings && _settings->displayInfo()) {
		int y = 0;
		foreach (QString info, _infos) {
#warning /// @todo assign size on font by default
			PhGraphicText gInfo(&_infoFont, info, 0, y, 300, 100);
			gInfo.setZ(10);
			gInfo.setColor(Qt::red);
			gInfo.draw();
			y += gInfo.getHeight();
		}
	}

	_frameTickCounter.tick();
}

void PhGraphicView::onRefresh()
{
//#if defined(Q_OS_MAC)
//	if(qApp->hasPendingEvents()) // qApp is a global pointer to the application
//		return;
//#endif
	//PHDEBUG ;
	updateGL();
}


