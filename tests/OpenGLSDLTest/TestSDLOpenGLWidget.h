#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QImage>
#include "phglwidget.h"
#include "glu.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"


#include <QFont>
#include <QLabel>
#include <QPainter>

class TestSDLOpenGLWidget : public PhGLWidget
{
    Q_OBJECT
public:
    explicit TestSDLOpenGLWidget(QWidget *parent = 0);
    void initializeGL();
    void paintGL();

private :
    GLuint texture;			// This is a handle to our texture object
    float x;
    float y;

};

#endif // MYWINDOW_H
