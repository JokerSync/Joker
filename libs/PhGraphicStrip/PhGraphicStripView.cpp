/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

//#include <QTime>

//#include "SDL/SDL.h"

#include "PhGraphicStripView.h"




PhGraphicStripView::PhGraphicStripView(QWidget *parent)
	: PhGraphicView( parent )
{
    // This is used to make some time-based test
	_test = new QTime();
	_test->start();
}


bool PhGraphicStripView::init()
{
	QString fontFile = "Bedizen.ttf";
	// Load font
	_currentFont = new PhFont(fontFile, 150);

	qDebug() << "PhGraphicStripView::init()";

	//This clear the data stored
	clearData();

//	int max = _controller->getDoc().getTexts().count();
//	QProgressDialog barTest("Création des textures","Ok", 0, max, this);


//	barTest.move(400,400);
//	 barTest.show();

	float h = this->height();
	float hstrip = this->height();
	float w = this->width();
	float tu = w/h;

	int i = 0;
	qDebug() << "load the text" ;
	//Load the all text
//	foreach(PhStripText * text, _controller->getDoc().getTexts())
//	{
//		qDebug() << "on rentre" ;
//		//barTest.setValue(i);

//		//h is the window height, hstrip is the strip height
//		//hstrip/16 correspond to the upper alpha line of the strip
//		//hstrip/8 correspond to the two alpha lines of the strip (up & down)
//		//it->getTrack() is the position on the strip (0 is the upper on)
//		//we split   in 3 because we are using 3 tracks on the strip
//		int y = h - (hstrip - hstrip/16) + ((hstrip - hstrip/4)/3)*text->getTrack() + 30;

//		//Display the name only if the setence is standalone
//		if (text->isSimple()){
//			int nameWidth = (text->getPeople().getName().length() + 1) * 10;
//			_texts.push_back(new PhGraphicText( _fonts.first(),text->getPeople().getName(),
//											   (text->getTimeIn() - _controller->getDoc().getLastPosition()) * 20 - nameWidth - 10, y, -1,
//											   nameWidth, 30, 1, 1, new QColor(text->getPeople().getColor())));
//		}
//		_texts.push_back(new PhGraphicText( _currentFont, text->getContent(),
//										   (text->getTimeIn() - _controller->getDoc().getLastPosition()) * 20, y , -1,
//											(text->getTimeOut() - text->getTimeIn()) * 20, hstrip / 5 , 1, 1, new QColor(text->getPeople().getColor())));
//	  //        if (i % (max / 20) == 0){
//	  //            QApplication::processEvents();
//	  //        }
//	   //       i++;
//	}

    //Set a default number of strip repetition
	int nbRythmo = this->width()/60;

    //Load the strip background
	_stripBackgroundImage = new PhGraphicImage("motif-240.png");
	_stripBackgroundImage->setTextureCoordinate(tu, 1);
	_stripBackgroundImage->init();

	//Load the cuts
//	foreach(PhStripCut * cut, _controller->getDoc().getCuts())
//	{
//		PhGraphicSolidRect *rect = new PhGraphicSolidRect((cut->getTimeIn() - _controller->getDoc().getLastPosition()) * 20, 0, -2,
//														  2, hstrip, new QColor(0, 0, 0));
//		_cuts.push_back(rect);
//	}

	return true;
}

void PhGraphicStripView::paint()
{
	qDebug() << "PhGraphicStripView::paint()";

	//Set the background color to white
	glClearColor(1,0,0,1);

	//Store the OpenGL context height in a variable in order to
	//limit functions call
	float h = this->height();
	float w = h*2/3;
	//Time-based test
	  qDebug() << _test->elapsed(); //<< " : " << _xmove;

//	//Set the deplacement size of the strip
//	if (_shouldmove){
//		_xmove -= 8;
//		_xMoveStrip -= 8;
//		// if the strip moved of more than 1 X strip's width it came back
//		if(_xMoveStrip <= -240 || _xMoveStrip >= 240)
//			_xMoveStrip = 0;
//	}

//    //Draw Backgroung Picture

	_stripBackgroundImage->setSize(w, h);
	_stripBackgroundImage->draw();

//    foreach(PhGraphicImage * it, _strips)
//    {
//		//it->draw(_xMoveStrip);
//		it->draw();
//    }
//	int x = 0;
//	int y = 0;
//	foreach(PhGraphicText * it, _texts)
//	{
//		//it->draw(_xmove);
//		qDebug() << "Draw PhGraphicText" ;
////		it->setX(x);
////		it->setY(y);
////		it->setWidht(w);
//		it->draw();
//	}
//	foreach(PhGraphicRect * it, _cuts)
//	{
//		//it->draw(_xmove);
//		it->draw();
//	}
}


//void PhGraphicStripView::stopScroll()
//{
//	qDebug() << "Stop Scrolling";
//	setScroll(false);
//}

void PhGraphicStripView::clearData()
{
	foreach(PhGraphicRect * it, _cuts)
	{
		delete it;
	}
	foreach(PhGraphicText * it, _texts)
	{
		delete it;
	}
	_cuts.clear();
	_texts.clear();
}


bool PhGraphicStripView::setCurrentFont(QString fontFile)
{
	if(!QFile::exists(fontFile))
	{
		qDebug() << "File does'nt exists : " << fontFile;
		return false;
	}
	_currentFont = new PhFont(fontFile, 150);

	return true;
}

//PhFont * PhGraphicStripView::getCurrentFont(){
//	return _currentFont;
//}


//void PhGraphicStripView::changeScroll()
//{
//	_shouldmove = ! _shouldmove;
//}


//void PhGraphicStripView::setScroll(bool shouldScroll)
//{
//	if(shouldScroll)
//		qDebug() << "The strip should scroll";
//	else
//		qDebug() << "The strip should NOT scroll";

//	_shouldmove = shouldScroll;
//}

//void PhGraphicStripView::setNaturalScroll(bool naturalScroll)
//{
//	_naturalScroll = naturalScroll;
//}

//void PhGraphicStripView::toggleNaturalScrolling()
//{
//	_naturalScroll = ! _naturalScroll;
//}

//void PhGraphicStripView::setController(PhGraphicStripController * controller)
//{
//	_controller = controller;
//}

//void PhGraphicStripView::connectSlots(){
//	connect(_controller, SIGNAL(docChanged()), this, SLOT(initializeGL()));
//	connect(_controller, SIGNAL(docChanged()), this, SLOT(stopScroll()));
//}

//void PhGraphicStripView::setXmove(int n)
//{
//	if (_naturalScroll)
//	{
//		_xmove -= n;
//		_xMoveStrip -= n;
//		if(_xMoveStrip <= -240 || _xMoveStrip >= 240)
//			_xMoveStrip = 0;
//	}
//	else
//	{
//		_xmove += n;
//		_xMoveStrip += n;
//		if(_xMoveStrip <= -240 || _xMoveStrip >= 240)
//			_xMoveStrip = 0;
//	}
//}


PhStripDoc *PhGraphicStripView::getDoc()
{
	return _doc;
}
