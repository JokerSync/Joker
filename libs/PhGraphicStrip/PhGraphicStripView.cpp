/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QFile>

#include "PhTools/PhDebug.h"
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
	PHDEBUG << "PhGraphicStripView::init()";

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
		delete gPeople;
	_graphicPeoples.clear();

	foreach(PhGraphicRect * gCut, _graphicCuts.values())
		delete gCut;
	_graphicCuts.clear();

	foreach(PhGraphicText * gText, _graphicTexts.values())
		delete gText;
	_graphicTexts.clear();

	foreach(PhGraphicLoop * gLoop, _graphicLoops.values())
		delete gLoop;
	_graphicLoops.clear();

	foreach(PhGraphicRect * gOff, _graphicOffs.values())
		delete gOff;
	_graphicOffs.clear();
}


bool PhGraphicStripView::setCurrentFont(QString fontFile)
{
	PHDEBUG << "setCurrentFont : " << fontFile;

	if(!QFile::exists(fontFile))
	{
		PHDEBUG << "File doesn't exists : " << fontFile;
		return false;
	}
	_currentFont = new PhFont(fontFile, 150);

	// TODO : redraw all texts

	return _currentFont->init();
}

void PhGraphicStripView::updateView()
{
	PHDEBUG << "updateView";
	if(!_currentFont)
	{
		PHDEBUG << "The font has not been initialised";
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

	//Load the loops
	foreach(PhStripLoop * loop, _doc.getLoops())
	{
		PhGraphicLoop *gLoop = new PhGraphicLoop();
		gLoop->setColor(QColor(0, 0, 0, 1));
		_graphicLoops[loop] = gLoop;
	}

	//Load the offs
	foreach(PhStripOff * off, _doc.getOffs())
	{
		PhGraphicSolidRect *gOff = new PhGraphicSolidRect();
		gOff->setColor(QColor(0, 0, 0));
		gOff->setZ(-2);

		_graphicOffs[off] = gOff;
	}

	PHDEBUG << "updateView ok";

}

PhTime lastTime = -1;
void PhGraphicStripView::paint()
{
	_clock.tick(60);

	long pixelPerFrame = 12;
	int fps = PhTimeCode::getFps(_clock.getTCType());
	long offset = _clock.time() * pixelPerFrame * fps / _clock.timeScale();
	long width = this->width();
	long height = this->height();

	//Set the background color to red
	glClearColor(1,0,0,1);

    //Draw backgroung picture
	int n = width / height + 2; // compute how much background repetition do we need
	_stripBackgroundImage->setTextureCoordinate(n, 1);
	long leftBG = 0;
	if(left >= 0)
		leftBG -= offset % height;
	else
		leftBG -= height - ((-offset) % height);
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
		gText->setX(text->getTimeIn() * pixelPerFrame - offset);
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
			gPeople->setX(text->getTimeIn() * pixelPerFrame - offset - gPeople->getWidth() - spaceBetweenPeopleAndText);
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
		gCut->setX(cut->getTimeIn() * pixelPerFrame - offset);
		gCut->setWidth(2);
		gCut->draw();
	}

	foreach(PhStripLoop * loop, _doc.getLoops())
	{
		PhGraphicLoop * gLoop = _graphicLoops[loop];
		gLoop->setX(loop->getTimeIn() * pixelPerFrame - offset);
		gLoop->setHThick(height/40);
		gLoop->setHeight(height);
		gLoop->setCrossHeight(height / 4);
		gLoop->setWidth(height / 4);

		gLoop->draw();
	}

	foreach(PhStripOff * off, _doc.getOffs())
	{
		PhGraphicRect *gOff = _graphicOffs[off];
		gOff->setX(off->getTimeIn() * pixelPerFrame - offset);
		gOff->setY(off->getTrack() * trackHeight + trackHeight);
		gOff->setHeight( trackHeight / 10);
		gOff->setWidth((off->getTimeOut() - off->getTimeIn()) * pixelPerFrame);

		gOff->draw();
	}
}
