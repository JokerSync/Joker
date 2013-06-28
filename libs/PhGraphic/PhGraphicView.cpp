#include "PhGraphicView.h"


PhGraphicView::PhGraphicView( QWidget *parent, QString name)
    : QGLWidget(parent)
{
    move = true;
    setWindowTitle(name);
    t_Timer = new QTimer(this);
    connect(t_Timer, SIGNAL(timeout()), this, SLOT(onRefresh()));
    t_Timer->start( 0);
    this->_context = new PhGraphicContext(this);
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
    initializeGL();
}

void PhGraphicView::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
    case Qt::Key_P:
        this->_context->saveToPNG();
        break;
    }
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

