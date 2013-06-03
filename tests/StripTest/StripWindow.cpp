/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QTime>


#include "SDL/SDL.h"

#include "StripWindow.h"



StripWindow::StripWindow(QWidget *parent, PhString file)
    : PhGraphicView( parent, "StripTest")
{
    xmove = this->width() * 1.5;
    resize(640,360);
    _isFS = false;

    _fonts.push_back( new PhFont("../Resources/fonts/zoinks.ttf", 100));
    _fonts.push_back( new PhFont("../Resources/fonts/Arial.ttf", 100));
    _currentFont = _fonts.last();

    _firstload = true;
    openFile(file);
    _firstload = false;
}


void StripWindow::initializeGL()
{
    QTime *beg = new QTime();
    beg->start();

    int h = 0;

    glClearColor(1,1,1,0);
    h = this->height();

    int hstrip = h / 4;
    clearData();

    for(auto it : _doc->getTexts())
    {
//        qDebug() << it->getTimeIn() << it->getPeople().getName() << ":" << it->getContent() << it->getTrack() << this->height() - (90 - it->getTrack()*30);
/*
        _texts.push_back(new PhGraphicText(it->getPeople().getName() + ":",
                                           it->getTimeIn() - 50 - 90000, this->height() - (90 - it->getTrack()*30), -1,
                                           50, 30, _fonts.first(), "vert"));
*/
        _texts.push_back(new PhGraphicText(it->getContent(),
                                           (it->getTimeIn() - _doc->getVideoTimestamp()) * 4, h - (hstrip - it->getTrack()*30), -1,
                                            (it->getTimeOut() - it->getTimeIn()) * 4, 30, _currentFont, "vert"));
    }


    _imgs.push_back(new PhGraphicImage("../Resources/img/rythmo-bg.png", 0,
                                       h - hstrip, -2,
                                       90, hstrip, "rose",
                                       100, 1));
    /*
    _imgs.push_back(new PhGraphicImage("../Resources/img/rythmo-bg.png",
                                       (this->width() - 480) / 2, 0, -2,
                                       480, 270, "rose",
                                       1, 1));
    */
    qDebug() << "It took " << beg->elapsed() << "ms";

}

void StripWindow::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void StripWindow::openFile(QString filename)
{
    this->_doc = new PhStripDoc(filename);
    clearData();
    if (!_firstload)
        initializeGL();

}

void StripWindow::clearData()
{
    _texts.clear();
    _imgs.clear();
}


QList<PhFont *> StripWindow::getFonts()
{
    return _fonts;
}

void StripWindow::setCurrentFont(PhFont * font){
    _currentFont = font;
    initializeGL();
}

void StripWindow::toggleFS(bool fs){
    _isFS = fs;
}

