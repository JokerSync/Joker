#ifndef MYWINDOW_H
#define MYWINDOW_H

#include "PhGraphic/phglwidget.h"
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
    GLuint textures[3];			// This is a handle to a tab of textures
    //GLuint texture;
    float xmove;
    float y;

};

#endif // MYWINDOW_H
