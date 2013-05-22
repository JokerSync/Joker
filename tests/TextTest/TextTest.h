#ifndef MYWINDOW_H
#define MYWINDOW_H

#include "PhGraphic/PhGraphicView.h"
#include "glu.h"
#include "SDL_image/SDL_image.h"
#include "SDL_ttf/SDL_ttf.h"



class TextTest : public PhGraphicView
{
    Q_OBJECT
public:
    explicit TextTest(QWidget *parent = 0);
    void initializeGL();
    void paintGL();

private :
    GLuint textures[3];			// This is a handle to a tab of textures
    //GLuint texture;
    float xmove;
    float y;

};

#endif // MYWINDOW_H
