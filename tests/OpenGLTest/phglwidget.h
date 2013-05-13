#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QtOpenGL>
#include <QGLWidget>

#include <stdio.h>
#include <stdlib.h>

#include <gl.h>
#include <glu.h>

class PhGLWidget : public QGLWidget
{
    Q_OBJECT
public:
    explicit PhGLWidget(QWidget *parent = 0, char *name = 0);
    virtual void initializeGL() = 0;
    virtual void resizeGL(int width, int height) = 0;
    virtual void paintGL() = 0;
    virtual void keyPressEvent( QKeyEvent *keyEvent );

    void toggleFullWindow();


public slots:
    virtual void onRefresh();

private:
    bool b_Fullscreen;
    QTimer *t_Timer;

};


#endif // MYGLWIDGET_H
