#include "PhGraphicView.h"


PhGraphicView::PhGraphicView( QWidget *parent, QString name)
    : QGLWidget(parent)
{
	t_Timer = new QTimer(this);
	//connect(t_Timer, SIGNAL(timeout()), this, SLOT(onRefresh()));
    t_Timer->start( 0);
    this->_context = new PhGraphicContext(this);
}

void PhGraphicView::initializeGL()
{
	qDebug() << "PhGraphicView::initializeGL" ;
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
    glOrtho(0, width, height, 0, 0, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void PhGraphicView::paintGL()
{
	qDebug() << "PhGraphicView::paintGL" ;
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearColor(1,0,0,1);
	paint();
}

void PhGraphicView::onRefresh()
{
    if(qApp->hasPendingEvents()) // qApp is a global pointer to the application
        return;
    updateGL();
}


PhGraphicContext *PhGraphicView::getContext()
{
	return _context;
}


