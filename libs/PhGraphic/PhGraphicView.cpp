#include "PhTools/PhDebug.h"
#include "PhGraphicView.h"
#include <SDL2/SDL.h>

#if defined(Q_OS_MAC)
#include <SDL2_ttf/SDL_ttf.h>
#else
#include <SDL2/SDL_ttf.h>
#endif

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
	t_Timer->start(10);
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

void PhGraphicView::setSettings(QSettings *settings)
{
	_settings = settings;
	PHDBG(0) << "The refresh rate have changed. Set the property \"onRefreshTime\" and reload" << APP_NAME << "to apply changes";
	t_Timer->start(_settings->value("onRefreshTime", 10).toInt());
}

void PhGraphicView::paintGL()
{
	//PHDEBUG << "PhGraphicView::paintGL" ;
	beforePaint(60);

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f, 1.0f, 1.0f);
	paint();

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


