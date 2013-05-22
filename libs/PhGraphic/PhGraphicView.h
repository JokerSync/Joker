#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QtOpenGL>
#include <QGLWidget>

#include <stdio.h>
#include <stdlib.h>

#include <gl.h>
#include <glu.h>

class PhGraphicView : public QGLWidget
{
    Q_OBJECT
public:
    explicit PhGraphicView(QWidget *parent = 0, QString name = "");
    virtual void keyPressEvent( QKeyEvent *keyEvent );
    virtual void initializeGL() = 0;
    virtual void paintGL() = 0;
    virtual void resizeGL(int width, int height);
    void toggleFullWindow();
    void toggleMouvement();

public slots:
    virtual void onRefresh();


private:
    bool b_Fullscreen;
    QTimer *t_Timer;
protected:
    float xdelta;
    float ydelta;
    bool move;


};


#endif // MYGLWIDGET_H
