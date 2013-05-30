#include <QTime>

#include "glu.h"

#include "TextTestWindow.h"

TextTestWindow::TextTestWindow(QWidget *parent)
    : PhGraphicView( parent, "Premier affichage de dessin avec OpenGL et Qt")
{
    _stripWidth = 0;
    _stripHeight = 0;
    xmove = this->width() * 1.5;
    resize(640,360);
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
    _fonts.push_back( new PhFont("../Resources/fonts/zoinks.ttf", 100));
    _fonts.push_back( new PhFont("../Resources/fonts/Arial.ttf", 100));
    _texts.push_back(new PhGraphicText("New Police", 100, 100, -2, 100, 100, _fonts.first(), "vert"));
    _texts.push_back(new PhGraphicText("Same Police new message", 100, 200, -3, 500, 100, _fonts.first(), "vert"));
    _texts.push_back(new PhGraphicText("why Arial? ÁÁØØØ",
                                       0, 0, -4, 500, 100, _fonts.last(), "vert"));
    _imgs.push_back(new PhGraphicImage("../Resources/img/brique.png", 0,
                                       this->height() - 90, -1, 90, 90, "rose"));
}

void TextTestWindow::paintGL()
{

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    for(auto it : _texts)
    {
        it->draw();
    }
    for(auto it : _imgs)
    {
        it->draw();
    }


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

}


void TextTestWindow::setStripHeight(int h)
{
    _stripHeight = h;
}

int TextTestWindow::getStripHeight()
{
    return _stripHeight;
}
