#include "PhGraphicView.h"


PhGraphicView::PhGraphicView( QWidget *parent, QString name)
    : QGLWidget(parent)
{
    move = true;
    b_Fullscreen = false ;
    setWindowTitle(name);
    t_Timer = new QTimer(this);
    connect(t_Timer, SIGNAL(timeout()), this, SLOT(onRefresh()));
    t_Timer->start( 0);
    xdelta = 1;
    this->_context = new PhGraphicContext(this);
    }

void PhGraphicView::resizeGL(int width, int height)
{
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(40.0f,(GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    glOrtho(0, width, height, 0, 0, 10);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    qDebug() << width;
}

void PhGraphicView::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Space:
        toggleFullWindow();
        break;
    case Qt::Key_Q:
        close();
        break;
    case Qt::Key_S:
        toggleMouvement();
        break;
    case Qt::Key_Left:
        xdelta += 1;
        break;
    case Qt::Key_Right:
        xdelta -= 1;
        break;
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

void PhGraphicView::toggleFullWindow()
{
    if(b_Fullscreen)
    {
        showNormal();
        b_Fullscreen = false;
    }
    else
    {
        showFullScreen();
        b_Fullscreen = true;
    }
}

void PhGraphicView::toggleMouvement()
{
    if(move)
        move = false;
    else
        move = true;
}
