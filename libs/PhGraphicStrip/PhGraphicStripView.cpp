/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

//#include <QTime>

#include "PhGraphicStripView.h"




PhGraphicStripView::PhGraphicStripView(QWidget *parent)
	: PhGraphicView( parent ), _doc(this)
{
	// update the view content when the doc changes :
	this->connect(&_doc, SIGNAL(changed()), this, SLOT(updateView()));

    // This is used to make some time-based test
	_test = new QTime();
	_test->start();
}


bool PhGraphicStripView::init()
{
	qDebug() << "PhGraphicStripView::init()";

	// Load font
	if(!setCurrentFont("Bedizen.ttf"))
		return false;

	//This clear the data stored
	clearData();

//	int max = _controller->getDoc().getTexts().count();
//	QProgressDialog barTest("Création des textures","Ok", 0, max, this);


//	barTest.move(400,400);
//	 barTest.show();

    //Load the strip background
	_stripBackgroundImage = new PhGraphicImage("motif-240.png");
	_stripBackgroundImage->init();

	updateView();

	return true;
}

void PhGraphicStripView::paint()
{
//	qDebug() << "PhGraphicStripView::paint()";

	//Set the background color to white
	glClearColor(1,0,0,1);

	//Time-based test
//	qDebug() << _test->elapsed(); //<< " : " << _xmove;

//	//Set the deplacement size of the strip
//	if (_shouldmove){
//		_xmove -= 8;
//		_xMoveStrip -= 8;
//		// if the strip moved of more than 1 X strip's width it came back
//		if(_xMoveStrip <= -240 || _xMoveStrip >= 240)
//			_xMoveStrip = 0;
//	}

	float h = this->height();
	float w = this->width();

//    //Draw Backgroung Picture
	_stripBackgroundImage->setTextureCoordinate(1.2f * w / h, 1);
	_stripBackgroundImage->setSize(w, h);
	_stripBackgroundImage->draw();

	foreach(PhStripText * text, _doc.getTexts())
	{
		PhGraphicText* gText = _texts[text];

		gText->setY(text->getTrack() * h / 4);
		gText->setZ(-1);
		gText->setWidth(text->getTimeOut() - text->getTimeIn());
		gText->setHeight(h / 4);

		gText->draw();
	}

	foreach(PhStripCut * cut, _doc.getCuts())
	{
		PhGraphicRect * gCut = _cuts[cut];
		gCut->setHeight(h);

		gCut->draw();
	}
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

	if(!_currentFont->init())
		return false;

	return true;
}

void PhGraphicStripView::updateView()
{
	if(!_currentFont)
		return;

	clearData();

	//Load the all text
	foreach(PhStripText * text, _doc.getTexts())
	{
		//Display the name only if the setence is standalone
//			if (text->isSimple()){
//				int nameWidth = (text->getPeople().getName().length() + 1) * 10;
//				_texts.push_back(new PhGraphicText( _fonts.first(),text->getPeople().getName(),
//												   (text->getTimeIn() - _controller->getDoc().getLastPosition()) * 20 - nameWidth - 10, y, -1,
//												   nameWidth, 30, 1, 1, new QColor(text->getPeople().getColor())));
//			}
		PhGraphicText * gText = new PhGraphicText( _currentFont, text->getContent());
		gText->setX(text->getTimeIn());
		gText->setZ(-1);
		gText->setWidth(text->getTimeOut() - text->getTimeIn());
		gText->setColor(new QColor(text->getPeople().getColor()));
		gText->setFont(_currentFont);

		gText->init();

		_texts[text] = gText;
	}

	//Load the cuts
	foreach(PhStripCut * cut, _doc.getCuts())
	{
		PhGraphicSolidRect *gCut = new PhGraphicSolidRect();
		gCut->setColor(new QColor(0, 0, 0));
		gCut->setX(cut->getTimeIn());
		gCut->setWidth(2);

		gCut->setZ(-2);

		_cuts[cut] = gCut;
	}
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
	return &_doc;
}
