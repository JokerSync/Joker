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
     * @brief b_Fullscreen
     * allow us to know if the application is fullScreen
     */
	//PhGraphicContext * getContext();

protected:

	virtual bool init() = 0;

	virtual void paint() = 0;

	void initializeGL();

	/**
	 * @brief resizeGL
	 * @param width
	 * @param height
	 * handle the resize of the OpenGL context
	 */
	void resizeGL(int width, int height);

	void paintGL();

private slots:
    /**
     * @brief onRefresh
     */
	void onRefresh();


private:

    /**
     * @brief t_Timer
     * used to draw
     */
    QTimer *t_Timer;
protected:

    /**
     * @brief _context
     */
   // PhGraphicContext *_context;

};

#endif // PhGraphicView
