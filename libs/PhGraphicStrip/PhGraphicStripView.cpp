/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "PhGraphicStripView.h"

PhGraphicStripView::PhGraphicStripView(QWidget *parent)
	: PhGraphicView( parent ), _doc(this), _currentFont(NULL), _trackNumber(4)
{
	// update the view content when the doc changes :
	this->connect(&_doc, SIGNAL(changed()), this, SLOT(updateView()));

    // This is used to make some time-based test
	_test = new QTime();
	_test->start();
}

PhStripDoc *PhGraphicStripView::doc()
{
	return &_doc;
}

PhClock *PhGraphicStripView::clock()
{
	return &_clock;
}

bool PhGraphicStripView::init()
{
	qDebug() << "PhGraphicStripView::init()";

	// Load font
	if(!setCurrentFont("Bedizen.ttf"))
		return false;

	// Clear the data stored
	clearData();

    //Load the strip background
	_stripBackgroundImage = new PhGraphicImage("motif-240.png");
	_stripBackgroundImage->init();

	updateView();

	return true;
}

void PhGraphicStripView::clearData()
{
	foreach(PhGraphicText * gPeople, _graphicPeoples.values())
	{
		delete gPeople;
	}

	foreach(PhGraphicRect * gCut, _graphicCuts.values())
	{
		delete gCut;
	}
	foreach(PhGraphicText * gText, _graphicTexts.values())
	{
		delete gText;
	}
	_graphicPeoples.clear();
	_graphicCuts.clear();
	_graphicTexts.clear();
}


bool PhGraphicStripView::setCurrentFont(QString fontFile)
{
	qDebug() << "setCurrentFont : " << fontFile;

	if(!QFile::exists(fontFile))
	{
		qDebug() << "File doesn't exists : " << fontFile;
		return false;
	}
	_currentFont = new PhFont(fontFile, 150);

	// TODO : redraw all texts

	return _currentFont->init();
}

void PhGraphicStripView::updateView()
{
	qDebug() << "updateView";
	if(!_currentFont)
	{
		qDebug() << "The font has not been initialised";
		return;
	}

	clearData();

	// Load the peoples
	foreach(PhPeople * people, _doc.getPeoples())
	{
		PhGraphicText * gPeople = new PhGraphicText(_currentFont, people->getName());
		gPeople->setColor(QColor(people->getColor()));
		gPeople->setWidth(people->getName().length() * 16);

		gPeople->init();

		_graphicPeoples[people] = gPeople;
	}

	//Load the texts
	foreach(PhStripText * text, _doc.getTexts())
	{
		PhGraphicText * gText = new PhGraphicText( _currentFont, text->getContent());
		float track = text->getTrack();

		gText->setZ(-1);
		if(text->getPeople())
			gText->setColor(QColor(text->getPeople()->getColor()));
		gText->setFont(_currentFont);

		gText->init();

		_graphicTexts[text] = gText;
	}

	//Load the cuts
	foreach(PhStripCut * cut, _doc.getCuts())
	{
		PhGraphicSolidRect *gCut = new PhGraphicSolidRect();
		gCut->setColor(QColor(0, 0, 0));
		gCut->setZ(-2);

		_graphicCuts[cut] = gCut;
	}

	qDebug() << "updateView ok";
}

void PhGraphicStripView::paint()
{
	_clock.tick(60);

	float pixelPerFrame = 16;
//	float length = this->width() / pixelPerFrame;
	float fps = PhTimeCode::getFps(_clock.getTCType());
//	float t = _clock.time() * fps / _clock.timeScale();
	long left = _clock.time() * pixelPerFrame * fps / _clock.timeScale();
	long width = this->width();
	long height = this->height();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
//	glOrtho(t, t + length, _trackNumber, 0, 0, 10);
	glOrtho(left, left + width, height, 0, 0, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Set the background color to red
	glClearColor(1,0,0,1);

	//Time-based test
//	qDebug() << _test->elapsed(); //<< " : " << _xmove;

//    //Draw backgroung picture
	int n = width / height + 2; // compute how much background repetition do we need
	_stripBackgroundImage->setTextureCoordinate(n, 1);
	long leftBG = left;
	if(left >= 0)
		leftBG -= left % height;
	else
		leftBG -= height - ((-left) % height);
	_stripBackgroundImage->setX(leftBG);
	_stripBackgroundImage->setSize(height * n, height);
	_stripBackgroundImage->draw();

	int minSpaceBetweenPeople = 50;
	int spaceBetweenPeopleAndText = 4;
	PhStripText ** lastTextList = new PhStripText*[_trackNumber];
	for(int i = 0; i < _trackNumber; i++)
		lastTextList[i] = NULL;

	int trackHeight = height / _trackNumber;

	foreach(PhStripText * text, _doc.getTexts())
	{
		PhGraphicText* gText = _graphicTexts[text];

		int track = text->getTrack();
		gText->setX(text->getTimeIn() * pixelPerFrame);
		gText->setWidth((text->getTimeOut() - text->getTimeIn()) * pixelPerFrame);
		gText->setY(track * trackHeight);
		gText->setHeight(trackHeight);

		gText->draw();

		PhStripText * lastText = lastTextList[track];
		// Display the people name only if one of the following condition is true:
		// - it is the first text
		// - it is a different people
		// - the distance between the latest text and the current is superior to a limit
		if((lastText == NULL) || (lastText->getPeople() != text->getPeople()) || (text->getTimeIn() - lastText->getTimeOut() > minSpaceBetweenPeople))
		{
			PhGraphicText * gPeople = _graphicPeoples[text->getPeople()];
			gPeople->setX(text->getTimeIn() * pixelPerFrame - gPeople->getWidth() - spaceBetweenPeopleAndText);
			gPeople->setY(track * trackHeight);
			gPeople->setHeight(trackHeight / 2);

			gPeople->draw();
		}

		lastTextList[track] = text;
	}

	foreach(PhStripCut * cut, _doc.getCuts())
	{
		PhGraphicRect *gCut = _graphicCuts[cut];
		gCut->setHeight(height);
		gCut->setX(cut->getTimeIn() * pixelPerFrame);
		gCut->setWidth(pixelPerFrame / 4);
		gCut->draw();
	}
}
