#include <QDebug>

#include "dwwindow.h"
#include "ui_dwwindow.h"


DWwindow::DWwindow(int width, int height)
{
    resize(width, height);

    setWindowTitle(QString::fromUtf8("SDL Test"));

    this->testSDLWidget = new TestSDLWidget(300, 200, this);
    this->testSDLWidget->move(10, 10);

    char windowid[64];

#ifdef Q_WS_WIN
    sprintf(windowid, "SDL_WINDOWID=0x%lx", reinterpret_cast<qlonglong>(winId()));
#else
    sprintf(windowid, "SDL_WINDOWID=0x%lx", winId());
#endif

    qDebug() << "DWwindow id :" << windowid;
}

void DWwindow::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
    case Qt::Key_Escape:
        close();
        break;
    case Qt::Key_Space:
        close();
        break;
    case Qt::Key_Q:
        close();
        break;
    }
}
