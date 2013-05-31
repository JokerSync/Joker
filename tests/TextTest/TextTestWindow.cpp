#include <QTime>

#include "glu.h"

#include "TextTestWindow.h"

TextTestWindow::TextTestWindow(QWidget *parent)
    : PhGraphicView( parent, "Phonations")
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

    _texts.push_back(new PhGraphicText("Oh John, I love you so much.",
                                       150, 300, -1,
                                       300, 50, _fonts.first(), "vert"));
    _texts.push_back(new PhGraphicText("Molly :",
                                       80, 310, -4,
                                       60, 30, _fonts.last(), "vert"));

    /*
    _texts.push_back(new PhGraphicText("Same Police new message", 100, 200, -3, 500, 100, _fonts.first(), "vert"));



    _imgs.push_back(new PhGraphicImage("imagefile1", 0,
                                       this->height() - 90, -2,
                                       90, 90, "rose",
                                       (this->width() / 90 + 1), 1));
    _imgs.push_back(new PhGraphicImage("imagefile2",
                                       (this->width() - 480) / 2, 0, -2,
                                       480, 270, "rose",
                                       1, 1));
    //*/
}

void TextTestWindow::paintGL()
{

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    for(auto it : _imgs)
    {
        it->draw();
    }
    for(auto it : _texts)
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
