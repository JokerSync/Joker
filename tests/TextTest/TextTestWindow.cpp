#include <QTime>

#include "glu.h"

#include "TextTestWindow.h"

TextTestWindow::TextTestWindow(QWidget *parent)
    : PhGraphicView( parent, "Premier affichage de dessin avec OpenGL et Qt")
{
    _stripWidth = 0;
    _stripHeight = 0;
    xmove = this->width() * 1.5;
    resize(1000,600);
}


int TextTestWindow::getStripWidth()
{
    return _stripWidth;
}

void TextTestWindow::setStripWidth(int w)
{
    _stripWidth = w;
}

void TextTestWindow::initializeGL()
{
    _font = new PhFont("../Resources/fonts/zoinks.ttf", 1000);
    _text = new PhGraphicText("Test", 100, 100, -2, 800, 100, _font, "vert");
    _img = new PhGraphicImage("../Resources/img/brique.png", 200, 10, -1, 256, 256, "rose");
}

void TextTestWindow::paintGL()
{
    _img->draw();

    _text->draw();



//    // in order to start from the window's right border
//    // could have been 0 to start from left border
//    int x = this->width();
//    // refers to window size
//    int w = this->width();
//    int h = this->height();
//    if (xmove > x + w)
//        xmove = 0;

//    if (move){
//        xmove += xdelta;
//    }



//    // Text texture
//    glBindTexture(GL_TEXTURE_2D, textures[2]);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//    glEnable(GL_TEXTURE_2D);
//    glBegin(GL_QUADS); 	//Begining the cube's drawing

//    glTexCoord3i(0, 0, 1);glVertex3i(x - xmove, h-h/4, -1);
//    glTexCoord3i(1, 0, 1);glVertex3i(x + w - xmove, h-h/4, -1);

//    glTexCoord3i(1, 1, 1);glVertex3i(x + w - xmove, h, -1);
//    glTexCoord3i(0, 1, 1);glVertex3i(x - xmove, h, -1);

//    glEnd();
//    glDisable(GL_TEXTURE_2D);

}


void TextTestWindow::setStripHeight(int h)
{
    _stripHeight = h;
}

int TextTestWindow::getStripHeight()
{
    return _stripHeight;
}
