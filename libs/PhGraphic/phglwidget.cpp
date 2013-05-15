#include "PhGLWidget.h"


PhGLWidget::PhGLWidget( QWidget *parent, QString name)
    : QGLWidget(parent)
{
    move = true;
    b_Fullscreen = false ;
    setWindowTitle(name);
    t_Timer = new QTimer(this);
    connect(t_Timer, SIGNAL(timeout()), this, SLOT(onRefresh()));
    t_Timer->start( 0);
    xdelta = 0.05;
    ydelta = 0.0;
}

void PhGLWidget::resizeGL(int width, int height)
{
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40.0f,(GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void PhGLWidget::keyPressEvent(QKeyEvent *keyEvent)
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
    case Qt::Key_Up:
        if (ydelta <= 0)
            ydelta = 0.05;
        xdelta = 0;
        break;
    case Qt::Key_Down:
        if (ydelta >= 0)
            ydelta = -0.05;
        xdelta = 0;
        break;
    case Qt::Key_Left:
        if (xdelta >= 0)
            xdelta = -0.05;
        ydelta = 0;
        break;
    case Qt::Key_Right:
        if (xdelta <= 0)
            xdelta = 0.05;
        ydelta = 0;
        break;
    }
}

void PhGLWidget::onRefresh()
{
    if(qApp->hasPendingEvents()) // qApp is a global pointer to the application
        return;
    updateGL();
}

void PhGLWidget::toggleFullWindow()
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

void PhGLWidget::toggleMouvement()
{
    if(move)
        move = false;
    else
        move = true;
}
