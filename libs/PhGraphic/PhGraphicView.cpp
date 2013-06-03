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


    QAction *quit = new QAction("&Quit", this);

    _menu = new QMenu(parent);
    _menu->addMenu("&File");
    _menu->addAction(quit);

    qDebug() << "Menu";

    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
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


