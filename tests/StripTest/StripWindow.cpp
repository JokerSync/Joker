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
    // This is used to make some time-based test
    _test = new QTime();
    _test->start();

    // This is a preload of default fonts
    _fonts.push_back( new PhFont(QCoreApplication::applicationDirPath() + "/../Resources/fonts/LTE50198.ttf", 150));
    _fonts.push_back( new PhFont(QCoreApplication::applicationDirPath() + "/../Resources/fonts/zoinks.ttf", 150));
    _fonts.push_back( new PhFont(QCoreApplication::applicationDirPath() + "/../Resources/fonts/Arial.ttf", 150));
    _fonts.push_back( new PhFont(QCoreApplication::applicationDirPath() + "/../Resources/fonts/Bedizen.ttf", 150));
    _fonts.push_back( new PhFont("/Library/Fonts/Apple Chancery.ttf", 150));

    _currentFont = _fonts.first();
    _xmove = 0;
    _xMoveStrip = 0;

    // The strip shouldn't move until a file is loaded
    _shouldmove = false;
    _firstload = true;

    // This open a file
    openFile(file);
}


void StripWindow::initializeGL()
{
    //Set the background color to white
    glClearColor(1,1,1,0);

    //Store the OpenGL context height in a variable in order to
    //limit functions call
    int h = this->height();

    //Set the ratio between the strip's height and the OpenGL height
    int hstrip = h;

    //This clear the data stored
    clearData();

    if(!_doc->getTitle().isNull())
        _xmove = - _doc->getLastPosition();
    //Load the all text
    for(auto it : _doc->getTexts())
    {
        //h is the window height, hstrip is the strip height
        //hstrip/16 correspond to the upper alpha line of the strip
        //hstrip/8 correspond to the two alpha lines of the strip (up & down)
        //it->getTrack() is the position on the strip (0 is the upper on)
        //we split in 3 because we are using 3 tracks on the strip
        int y = h - (hstrip - hstrip/16) + ((hstrip - hstrip/4)/3)*it->getTrack() + 30;        
        //Display the name only if the setence is standalone
        // Not very accurate
        if (it->isSimple()){
            int nameWidth = (it->getPeople().getName().length() + 1) * 10;
            _texts.push_back(new PhGraphicText(it->getPeople().getName(),
                                               (it->getTimeIn() - _doc->getLastPosition()) * 20 - nameWidth - 10, y, -1,
                                               nameWidth, 30, _fonts.first(), it->getPeople().getColor()));
        }
        _texts.push_back(new PhGraphicText(it->getContent(),
                                           (it->getTimeIn() - _doc->getLastPosition()) * 20, y , -1,
                                           (it->getTimeOut() - it->getTimeIn()) * 20, hstrip / 5 , _currentFont, it->getPeople().getColor()));
    }

    //Set a default number of strip repetition
    int nbRythmo = this->width()/240 + 2;

    //Load the strip on the right
    _imgs.push_back(new PhGraphicImage(QCoreApplication::applicationDirPath() + "/../Resources/img/motif-240.png",
                                       0, h - hstrip, -3,
                                       240, hstrip, PhColor("white"),
                                       nbRythmo, 1));

    for(auto it : _doc->getCuts())
    {
        _cuts.push_back(new PhGraphicTexturedRect((it->getTimeIn() - _doc->getLastPosition()) * 20, 0, -2,
                                                  8, hstrip,
                                                  PhColor("red")));
    }

}

void StripWindow::paintGL()
{



    //Time-based test
    //qDebug() << _test->elapsed() << " : " << _xmove;

    //Clear the buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    //Set the deplacement size of the strip
    if (_shouldmove){
        _xmove -= 8;
        _xMoveStrip -= 8;
        // if the strip moved of more than 1 X strip's width it came back
        if(_xMoveStrip <= -240)
            _xMoveStrip = 0;
    }

    //Draw Objects
    for(auto it : _imgs)
    {
        it->draw(_xMoveStrip);
    }
    for(auto it : _texts)
    {
        it->draw(_xmove);
    }
    for(auto it : _cuts)
    {
        it->draw(_xmove);
    }

}


void StripWindow::openFile(QString filename)
{
    _xmove = 0;
    this->_doc = new PhStripDoc(filename);
    if (!_firstload)
        initializeGL();
    _firstload = false;

}


void StripWindow::clearData()
{
    for(auto it : _imgs)
    {
        delete it;
    }
    for(auto it : _texts)
    {
        delete it;
    }
    _imgs.clear();
    _texts.clear();
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
