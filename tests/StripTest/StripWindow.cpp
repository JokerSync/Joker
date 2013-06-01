/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "SDL/SDL.h"

#include "StripWindow.h"



StripWindow::StripWindow(QWidget *parent, char *file)
    : PhGraphicView( parent, "Phonations")
{
    xmove = this->width() * 1.5;
    setWindowState();
    resize(1280,360);
    _doc = new PhStripDoc(file);
}


void StripWindow::initializeGL()
{

    _fonts.push_back( new PhFont("../Resources/fonts/zoinks.ttf", 100));
    _fonts.push_back( new PhFont("../Resources/fonts/Arial.ttf", 100));

    for(auto it : _doc->getTexts())
    {
        qDebug() << it->getTimeIn() << it->getPeople().getName() << ":" << it->getContent() << it->getTrack() << this->height() - (90 - it->getTrack()*30);
/*
        _texts.push_back(new PhGraphicText(it->getPeople().getName() + ":",
                                           it->getTimeIn() - 50 - 90000, this->height() - (90 - it->getTrack()*30), -1,
                                           50, 30, _fonts.first(), "vert"));
*/
        _texts.push_back(new PhGraphicText(it->getContent(),
                                           (it->getTimeIn() - 90000) * 3, this->height() - (90 - it->getTrack()*30), -1,
                                           (it->getTimeOut() - 90000) * 3, 30, _fonts.last(), "vert"));
    }

    _imgs.push_back(new PhGraphicImage("strip", 0,
                                       this->height() - 90, -2,
                                       90, 90, "rose",
                                       (this->width() / 90 + 1), 1));
    _imgs.push_back(new PhGraphicImage("videoscreenshoot",
                                       (this->width() - 480) / 2, 0, -2,
                                       480, 270, "rose",
                                       1, 1));

    /*

    _texts.push_back(new PhGraphicText("Molly :",
                                       80, 310, -4,
                                       60, 30, _fonts.last(), "vert"));

    _texts.push_back(new PhGraphicText("Same Police new message", 100, 200, -3, 500, 100, _fonts.first(), "vert"));





    //*/
}

void StripWindow::paintGL()
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
