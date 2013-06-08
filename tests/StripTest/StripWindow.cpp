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

    _currentFont = _fonts.first();

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
    int nbRythmo = 100;

    //Set a number adapted to the loaded file
    // 4 sec = 1920px => 1 sec = 640px
    if(_doc->getTitle() != NULL)
        nbRythmo = _doc->getDuration() * 640 / 240 + 2;

    //Load the strip on the right
    _imgs.push_back(new PhGraphicImage(QCoreApplication::applicationDirPath() + "/../Resources/img/motif-240.png",
                                       0, h - hstrip, -2,
                                       240, hstrip, PhColor("white"),
                                       nbRythmo, 1));

    //Load the strip on the left
    _imgs.push_back(new PhGraphicImage(QCoreApplication::applicationDirPath() + "/../Resources/img/motif-240.png",
                                       0, h - hstrip, -2,
                                       240, hstrip, PhColor("white"),
                                       -20, 1));

}

void StripWindow::paintGL()
{

    //Time-based test
    //qDebug() << _test->elapsed() << " : " << _xmove;

    //Clear the buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Set the deplacement size of the set
    if (_shouldmove)
        _xmove -= 8;

    //Draw Objects
    for(auto it : _imgs)
    {
        it->draw(_xmove);
    }
    for(auto it : _texts)
    {
        it->draw(_xmove);
    }

}

/**
 * @brief StripWindow::openFile
 * @param filename
 * Set the position to 0, load a new PhStripDoc, call initializeGL if another file was loaded
 */
void StripWindow::openFile(QString filename)
{
    _xmove = 0;
    this->_doc = new PhStripDoc(filename);
    if (!_firstload)
        initializeGL();
    _firstload = false;

}

/**
 * @brief StripWindow::clearData
 *Clear _texts and _imgs
 */
void StripWindow::clearData()
{
    _texts.clear();
    _imgs.clear();
}

/**
 * @brief StripWindow::getFonts
 * @return QList<PhFont *>
 */
QList<PhFont *> StripWindow::getFonts()
{
    return _fonts;
}

/**
 * @brief StripWindow::setCurrentFont
 * @param font
 * Set the current font with the font in param and re-initialize the OpenGL content
 */
void StripWindow::setCurrentFont(PhFont * font){
    _currentFont = font;
    initializeGL();
}

/**
 * @brief StripWindow::getCurrentFont
 * @return PhFont
 */
PhFont * StripWindow::getCurrentFont(){
    return _currentFont;
}

/**
 * @brief StripWindow::changeScroll
 * Toggle between Play and Pause
 */
void StripWindow::changeScroll()
{
    _shouldmove = ! _shouldmove;
}

/**
 * @brief StripWindow::setScroll
 * @param shouldScroll
 * Set the scroll information
 */
void StripWindow::setScroll(bool shouldScroll)
{
    _shouldmove = shouldScroll;
}

/**
 * @brief StripWindow::setXmove
 * @param n
 * Used to go to a precize position
 */
void StripWindow::setXmove(int n){
    _xmove += -n;
}

/**
 * @brief StripWindow::getDoc
 * @return PhStripDoc
 */
PhStripDoc *StripWindow::getDoc()
{
    return _doc;
}
