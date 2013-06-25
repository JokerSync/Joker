/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QTime>


#include "SDL/SDL.h"

#include "StripWindow.h"

#include "SampleListener.h"

using namespace Leap;


StripWindow::StripWindow(QWidget *parent, PhString file)
    : PhGraphicView( parent, "StripTest")
{
    controller.addListener(listener);
    _naturalScroll = true;


    // This is used to make some time-based test
    _test = new QTime();
    _test->start();

    // This is a preload of default fonts
    _fonts.push_back( new PhFont(QCoreApplication::applicationDirPath() + "/../Resources/fonts/LTE50198.ttf", 150));
    _fonts.push_back( new PhFont(QCoreApplication::applicationDirPath() + "/../Resources/fonts/zoinks.ttf", 150));
    _fonts.push_back( new PhFont(QCoreApplication::applicationDirPath() + "/../Resources/fonts/Bedizen.ttf", 150));

    // This is a routine witch load Apple system TTF fonts

    // Set the location
    PhString sourceFolder = "/Library/Fonts/";
    QDir sourceDir(sourceFolder);
    // List all files
    QStringList files = sourceDir.entryList(QDir::Files);
    // browse files and select only TTF ones
    for(int i = 0; i< files.count(); i++)
    {
        if(files[i].split(".").at(1) == "ttf")
        {
            _fonts.push_back(new PhFont("/Library/Fonts/" + files[i], 150));
        }
    }

    // Set current font
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

    int max = _doc->getNbTexts();
    QProgressDialog barTest("Création des textures","Ok", 0, max, this);

    barTest.move(400,400);

    //barTest.show();

    int i = 0;
    //Load the all text
    for(auto it : _doc->getTexts())
    {
        barTest.setValue(i);

        //h is the window height, hstrip is the strip height
        //hstrip/16 correspond to the upper alpha line of the strip
        //hstrip/8 correspond to the two alpha lines of the strip (up & down)
        //it->getTrack() is the position on the strip (0 is the upper on)
        //we split in 3 because we are using 3 tracks on the strip
        int y = h - (hstrip - hstrip/16) + ((hstrip - hstrip/4)/3)*it->getTrack() + 30;        

        //Display the name only if the setence is standalone
        if (it->isSimple()){
            int nameWidth = (it->getPeople().getName().length() + 1) * 10;
            _texts.push_back(new PhGraphicText(it->getPeople().getName(),
                                               (it->getTimeIn() - _doc->getLastPosition()) * 20 - nameWidth - 10, y, -1,
                                               nameWidth, 30, _fonts.first(), it->getPeople().getColor()));
        }
        _texts.push_back(new PhGraphicText(it->getContent(),
                                           (it->getTimeIn() - _doc->getLastPosition()) * 20, y , -1,
                                           (it->getTimeOut() - it->getTimeIn()) * 20, hstrip / 5 , _currentFont, it->getPeople().getColor()));
        if (i % (max / 10) == 0){
            //QApplication::processEvents();
        }
        i++;
    }

    //Set a default number of strip repetition
    int nbRythmo = this->width()/60;

    //Load the strip
    _strips.push_back(new PhGraphicImage(QCoreApplication::applicationDirPath() + "/../Resources/img/motif-240.png",
                                       -1000, h - hstrip, -3,
                                       240, hstrip, PhColor("white"),
                                       nbRythmo, 1));

    //Load the cuts
    for(auto it : _doc->getCuts())
    {
        _cuts.push_back(new PhGraphicTexturedRect((it->getTimeIn() - _doc->getLastPosition()) * 20, 0, -2,
                                                  2, hstrip,
                                                  PhColor("black")));
    }

}

void StripWindow::paintGL()
{

    //Controller.

    //Time-based test
    //qDebug() << _test->elapsed() << " : " << _xmove;

    if(controller.frame().fingers().count() == 1)
    {
        setXmove((controller.frame(1).fingers().leftmost().tipPosition().x - controller.frame().fingers().leftmost().tipPosition().x) * 4);
    }
    if(controller.frame().fingers().count() == 2)
    {
        setXmove(controller.frame().fingers().leftmost().tipPosition().x / 5);
    }


    //Clear the buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Set the deplacement size of the strip
    if (_shouldmove){
        _xmove -= 8;
        _xMoveStrip -= 8;
        // if the strip moved of more than 1 X strip's width it came back
        if(_xMoveStrip <= -240 || _xMoveStrip >= 240)
            _xMoveStrip = 0;
    }

    //Draw Objects
    for(auto it : _strips)
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
    _xmove = - _doc->getLastPosition();
}


void StripWindow::clearData()
{
    for(auto it : _cuts)
    {
        delete it;
    }
    for(auto it : _texts)
    {
        delete it;
    }
    _cuts.clear();
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

void StripWindow::toggleScrolling()
{
    _naturalScroll = ! _naturalScroll;
}

void StripWindow::setXmove(int n)
{
    if (_naturalScroll)
    {
        _xmove -= n;
        _xMoveStrip -= n;
        if(_xMoveStrip <= -240 || _xMoveStrip >= 240)
            _xMoveStrip = 0;
    }
    else
    {
        _xmove += n;
        _xMoveStrip += n;
        if(_xMoveStrip <= -240 || _xMoveStrip >= 240)
            _xMoveStrip = 0;
    }
}


PhStripDoc *StripWindow::getDoc()
{
    return _doc;
}
