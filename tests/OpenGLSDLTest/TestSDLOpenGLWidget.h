#ifndef MYWINDOW_H
#define MYWINDOW_H

#include "phglwidget.h"
#include "glu.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"



class TestSDLOpenGLWidget : public PhGLWidget
{
    Q_OBJECT
public:
    explicit TestSDLOpenGLWidget(QWidget *parent = 0);
    void initializeGL();
    void paintGL();

private :
    GLuint texture;			// This is a handle to our texture object
    SDL_Surface *surface;	// This surface will tell us the details of the image
    float x;
    float y;

};

#endif // MYWINDOW_H
