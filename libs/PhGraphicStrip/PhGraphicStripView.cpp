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

	float h = this->height();
	float w = this->width();

//    //Draw backgroung picture
	_stripBackgroundImage->setTextureCoordinate(1.2f * w / h, 1);
	_stripBackgroundImage->setSize(w, h);
	_stripBackgroundImage->draw();

	int trackNumber = 4;
	int minSpaceBetweenPeople = 10;
	int spaceBetweenPeopleAndText = 1;
	PhStripText ** lastTextList = new PhStripText*[trackNumber];
	for(int i = 0; i < trackNumber; i++)
		lastTextList[i] = NULL;

	foreach(PhStripText * text, _doc.getTexts())
	{
		PhGraphicText* gText = _graphicTexts[text];

		int track = text->getTrack();
		gText->setY(track * h / trackNumber);
		gText->setHeight(h / trackNumber);

		gText->draw();

		PhStripText * lastText = lastTextList[track];
		// Display the people name only if one of the following condition is true:
		// - it is the first text
		// - it is a different people
		// - the distance between the latest text and the current is superior to a limit
		if((lastText == NULL) || (lastText->getPeople() != text->getPeople()) || (text->getTimeIn() - lastText->getTimeOut() > minSpaceBetweenPeople))
		{
			PhGraphicText * gPeople = _graphicPeoples[text->getPeople()];
			gPeople->setX(text->getTimeIn() - gPeople->getWidth() - spaceBetweenPeopleAndText);
			gPeople->setY(track * h / trackNumber);
			gPeople->setHeight(h / trackNumber / 2);

			gPeople->draw();
		}

		lastTextList[track] = text;
	}

	foreach(PhStripCut * cut, _doc.getCuts())
	{
		PhGraphicRect * gCut = _graphicCuts[cut];
		gCut->setHeight(h);

		gCut->draw();
	}
}

void PhGraphicStripView::clearData()
{
	foreach(PhGraphicRect * it, _graphicCuts)
	{
		delete it;
	}
	foreach(PhGraphicText * it, _graphicTexts)
	{
		delete it;
	}
	_graphicCuts.clear();
	_graphicTexts.clear();
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

	// Load the peoples
	foreach(PhPeople * people, _doc.getPeoples())
	{
		PhGraphicText * gPeople = new PhGraphicText(_currentFont, people->getName());
		gPeople->setColor(new QColor(people->getColor()));
		gPeople->setWidth(people->getName().length() * 16);
		gPeople->setFont(_currentFont);

		gPeople->init();

		_graphicPeoples[people] = gPeople;
	}

	//Load the texts
	foreach(PhStripText * text, _doc.getTexts())
	{
		PhGraphicText * gText = new PhGraphicText( _currentFont, text->getContent());
		gText->setX(text->getTimeIn());
		gText->setZ(-1);
		gText->setWidth(text->getTimeOut() - text->getTimeIn());
		if(text->getPeople())
			gText->setColor(new QColor(text->getPeople()->getColor()));
		gText->setFont(_currentFont);

		gText->init();

		_graphicTexts[text] = gText;
	}

	//Load the cuts
	foreach(PhStripCut * cut, _doc.getCuts())
	{
		PhGraphicSolidRect *gCut = new PhGraphicSolidRect();
		gCut->setColor(new QColor(0, 0, 0));
		gCut->setX(cut->getTimeIn());
		gCut->setWidth(2);

		gCut->setZ(-2);

		_graphicCuts[cut] = gCut;
	}
}

PhStripDoc *PhGraphicStripView::getDoc()
{
	return &_doc;
}
