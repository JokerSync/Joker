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
    _test = new QTime();
    _test->start();

    _fonts.push_back( new PhFont("../Resources/fonts/LTE50198.ttf", 100));
    _fonts.push_back( new PhFont("../Resources/fonts/zoinks.ttf", 100));
    _fonts.push_back( new PhFont("../Resources/fonts/Arial.ttf", 100));
    _fonts.push_back( new PhFont("../Resources/fonts/Bedizen.ttf", 100));

    _currentFont = _fonts.last();
    _shouldmove = false;
    _firstload = true;
    openFile(file);
}


void StripWindow::initializeGL()
{
    QTime *beg = new QTime();
    beg->start();

    glClearColor(1,1,1,0);

    int h = this->height();
    int hstrip = h;


    clearData();

    for(auto it : _doc->getTexts())
    {
        //qDebug() << it->getTimeIn() << it->getPeople().getName() << ":" << it->getContent() << it->getTrack();
/*
        _texts.push_back(new PhGraphicText(it->getPeople().getName() + ":",
                                           it->getTimeIn() - 50 - 90000, this->height() - (90 - it->getTrack()*30), -1,
                                           50, 30, _fonts.first(), "vert"));
*/
        _texts.push_back(new PhGraphicText(it->getContent(),
                                           (it->getTimeIn() - _doc->getVideoTimestamp()) * 20, h - (hstrip - it->getTrack()*(hstrip /3)), -1,
                                            (it->getTimeOut() - it->getTimeIn()) * 20, hstrip / 3 , _currentFont, "vert"));
    }

    int nbRythmo = 10;

    // 4 sec = 1920px => 1 sec = 640px
    if(_doc->getTitle() != NULL)
        nbRythmo = _doc->getDuration() * 640 / 240 + 2;

    _imgs.push_back(new PhGraphicImage("../Resources/img/motif-240.png", 0,
                                       h - hstrip, -2,
                                       240, hstrip, "rose",
                                       nbRythmo, 1));
    /*
    _imgs.push_back(new PhGraphicImage("../Resources/img/rythmo-bg.png",
                                       (this->width() - 480) / 2, 0, -2,
                                       480, 270, "rose",
                                       1, 1));
    */
    //qDebug() << "It tooks " << beg->elapsed() << "ms to load text on texture";

}

void StripWindow::paintGL()
{

    //qDebug() << _test->elapsed() << " : " << _xmove;

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (_shouldmove)
        _xmove -= 8;

    for(auto it : _imgs)
    {
        it->draw(_xmove);
    }
    for(auto it : _texts)
    {
        it->draw(_xmove);
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
    _xmove = 0;
    this->_doc = new PhStripDoc(filename);
    clearData();
    if (!_firstload)
        initializeGL();
    _firstload = false;

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

PhFont * StripWindow::getCurrentFont(){
    return _currentFont;
}


void StripWindow::changeScroll()
{
    _shouldmove = ! _shouldmove;
}

void StripWindow::setScroll(bool shouldScroll)
{
    _shouldmove = shouldScroll;
}

void StripWindow::setXmove(int n){
    _xmove += -n;
}

PhStripDoc *StripWindow::getDoc()
{
    return _doc;
}
