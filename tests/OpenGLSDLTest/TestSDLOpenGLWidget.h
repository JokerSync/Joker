#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QImage>
#include "myGLWidget.h"
#include "glu.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"


#include <QFont>
#include <QLabel>
#include <QPainter>

class myWindow : public myGLWidget
{
    Q_OBJECT
public:
    explicit myWindow(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();

private :
    QLabel test;
    GLuint texture[1];
    float x;
};

#endif // MYWINDOW_H
