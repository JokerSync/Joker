#include "PhGLWidget.h"


PhGLWidget::PhGLWidget( QWidget *parent, char *name)
    : QGLWidget(parent)
{
    b_Fullscreen = false ;
    setWindowTitle(QString::fromUtf8(name));
    t_Timer = new QTimer(this);
    connect(t_Timer, SIGNAL(timeout()), this, SLOT(onRefresh()));
    t_Timer->start( 0);
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
