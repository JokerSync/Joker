#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QtOpenGL>
#include <QGLWidget>
#include <QMenu>
#include <QMenuBar>

#include <stdio.h>
#include <stdlib.h>

#include <gl.h>
#include <glu.h>

#include "PhGraphicContext.h"

class PhGraphicView : public QGLWidget
{
    Q_OBJECT
public:
    /**
     * @brief PhGraphicView
     * @param parent
     * @param name
     * Constructor
     */
    explicit PhGraphicView(QWidget *parent = 0, QString name = "");
    /**
     * @brief keyPressEvent
     * @param keyEvent
     * handle the key events
     */
    virtual void keyPressEvent( QKeyEvent *keyEvent );
    /**
     * @brief initializeGL
     * initialize the content
     */
    virtual void initializeGL() = 0;
    /**
     * @brief paintGL
     * draw the content
     */
    virtual void paintGL() = 0;

    virtual void setXmove(int px) = 0;
    /**
     * @brief resizeGL
     * @param width
     * @param height
     * handle the resize of the OpenGL context
     */
    virtual void resizeGL(int width, int height);
    /**
     * @brief toggleFullWindow
     * switch from fullscreen to normal window and vice-versa
     */
    void toggleFullWindow();
    /**
     * @brief toggleMouvement
     * switch from play to pause an vice-versa
     */
    void toggleScroll();
    /**
     * @brief b_Fullscreen
     * allow us to know if the application is fullScreen
     */
    PhGraphicContext * getContext();


public slots:
    /**
     * @brief onRefresh
     */
    virtual void onRefresh();


private:
    /**
     * @brief t_Timer
     * used to draw
     */
    QTimer *t_Timer;
protected:
    /**
     * @brief move
     * allow us to know if the strip is scrolling or not
     */
    bool move;
    /**
     * @brief _context
     */
    PhGraphicContext *_context;

};

#endif // PhGraphicView
