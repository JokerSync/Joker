/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QFile>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>

#include "PhTools/PhDebug.h"
#include "PhGraphicStrip.h"

PhGraphicStrip::PhGraphicStrip(QObject *parent) :
	QObject(parent),
	_doc(this),
	_currentFont(NULL),
	_trackNumber(4),
	_clock(_doc.getTCType())
{
	// update the  content when the doc changes :
	this->connect(&_doc, SIGNAL(changed()), this, SLOT(update()));

    // This is used to make some time-based test
	_test = new QTime();
	_test->start();
}

PhStripDoc *PhGraphicStrip::doc()
{
	return &_doc;
}

PhClock *PhGraphicStrip::clock()
{
	return &_clock;
}

bool PhGraphicStrip::setFont(QString fontName)
{
	PHDEBUG << fontName;
	QString fontFile = "~/Library/Fonts/" + fontName + ".ttf";
	if(!QFile::exists(fontFile))
	{
		fontFile = "/Library/Fonts/" + fontName + ".ttf";
		if(!QFile::exists(fontFile))
		{
			PHDEBUG << "Unable to find the font : " << fontName;
			fontFile = QCoreApplication::applicationDirPath() + "/../Resources/LTE50198.TTF";
			PHDEBUG << "A default font will be taken : " << fontFile;

			return false;
		}
	}


	PHDEBUG << "file : " << fontFile;

	_currentFont = new PhFont(fontFile, 150);

	PHDEBUG << "_currentFont value" << _currentFont->getFontName();

	// TODO : redraw all texts
	PHDEBUG << "currentFont init" << _currentFont->init();

	if(_currentFont->init())
	{
		update();
		return true;
	}
	else
		return false;
}


bool PhGraphicStrip::init()
{
	PHDEBUG << "PhGraphicStrip::init()";

	// Clear the data stored
	clearData();

    //Load the strip background
	_stripBackgroundImage = new PhGraphicImage(QCoreApplication::applicationDirPath() + "/../Resources/motif-240.png");
	_stripBackgroundImage->init();

	_stripSyncBar = new PhGraphicSolidRect();
	_stripSyncBar->setColor(QColor(225, 86, 108));


	return true;
}

void PhGraphicStrip::clearData()
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

void PhGraphicStrip::update()
{
	PHDEBUG << "update";
	if(!_currentFont)
	{
		PHDEBUG << "The font has not been initialised";
		return;
	}

	clearData();

/*	// Load the peoples
	foreach(PhPeople * people, _doc.getPeoples())
	{
		PhGraphicText * gPeople = new PhGraphicText(_currentFont, people->getName());
		gPeople->setColor(QColor(people->getColor()));
		gPeople->setWidth(people->getName().length() * 16);

		gPeople->init();

		_graphicPeoples[people] = gPeople;
	}

	PHDEBUG << "people loaded" ;

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

	PHDEBUG << "text loaded" ;

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

	PHDEBUG << "loops loaded" ;

	//Load the offs
	foreach(PhStripOff * off, _doc.getOffs())
	{
		PhGraphicSolidRect *gOff = new PhGraphicSolidRect();
		if(off->getPeople())
			gOff->setColor(QColor(off->getPeople()->getColor()));
		gOff->setZ(-2);

		_graphicOffs[off] = gOff;
	}

	PHDEBUG << "offs loaded" ;*/
}

void PhGraphicStrip::draw(int x, int y, int width, int height)
{
	//PHDEBUG << "time " << _clock.time() << " \trate " << _clock.rate();
	int loopCounter = 0;
	int offCounter = 0;
	int cutCounter = 0;

	_clock.tick(60);

	long pixelPerFrame = 12;
	int fps = PhTimeCode::getFps(_clock.timeCodeType());
	long syncBar_X_FromLeft = width / 6;
	long offset = _clock.time() * pixelPerFrame * fps / _clock.timeScale() - syncBar_X_FromLeft;
	//Compute the visible duration of the strip
	PhFrame stripDuration = width / pixelPerFrame;


	PhFrame frameIn = _clock.frame() - syncBar_X_FromLeft;
	PhFrame frameOut = _clock.frame() + stripDuration;

    //Draw backgroung picture
	int n = width / height + 2; // compute how much background repetition do we need
	_stripBackgroundImage->setTextureCoordinate(n, 1);
	long leftBG = 0;
	if(left >= 0)
		leftBG -= offset % height;
	else
		leftBG -= height - ((-offset) % height);
	_stripBackgroundImage->setX(x + leftBG);
	_stripBackgroundImage->setY(y);
	_stripBackgroundImage->setSize(height * n, height);
	_stripBackgroundImage->draw();

	_stripSyncBar->setSize(4, height);
	_stripSyncBar->setPosition(x + width/6, y, -1);
	_stripSyncBar->setSize(4, height);
	_stripSyncBar->draw();

	int minSpaceBetweenPeople = 50;
	int spaceBetweenPeopleAndText = 4;
	PhStripText ** lastTextList = new PhStripText*[_trackNumber];
	for(int i = 0; i < _trackNumber; i++)
		lastTextList[i] = NULL;

	int trackHeight = height / _trackNumber;

	foreach(PhStripText * text, _doc.getTexts())
	{
		PhGraphicText* gText = _graphicTexts[text];
		if(gText == NULL)
		{
			gText = new PhGraphicText( _currentFont, text->getContent());

			gText->setZ(-1);
			if(text->getPeople())
				gText->setColor(QColor(text->getPeople()->getColor()));
			gText->setFont(_currentFont);

			gText->init();

			_graphicTexts[text] = gText;
		}
		int track = text->getTrack();
		PhTime timeIn = text->getTimeIn();
		PhTime timeOut = text->getTimeOut();

		if( ! (((timeIn < frameIn) && (timeOut < frameIn)) || ((timeIn > frameOut) && (timeOut > frameOut))) )
		{
			gText->setX(x + timeIn * pixelPerFrame - offset);
			gText->setWidth((timeOut - timeIn) * pixelPerFrame);
			gText->setY(y + track * trackHeight);
			gText->setHeight(trackHeight);

			gText->draw();
		}

		PhStripText * lastText = lastTextList[track];
		// Display the people name only if one of the following condition is true:
		// - it is the first text
		// - it is a different people
		// - the distance between the latest text and the current is superior to a limit
		if((lastText == NULL) || (lastText->getPeople() != text->getPeople()) || (text->getTimeIn() - lastText->getTimeOut() > minSpaceBetweenPeople))
		{
			PhPeople * people = text->getPeople();
			PhGraphicText * gPeople = _graphicPeoples[people];
			if(gPeople == NULL)
			{
				gPeople = new PhGraphicText(_currentFont, people->getName());
				gPeople->setColor(QColor(people->getColor()));
				gPeople->setWidth(people->getName().length() * 16);

				gPeople->init();

				_graphicPeoples[people] = gPeople;
			}
			gPeople->setX(x + text->getTimeIn() * pixelPerFrame - offset - gPeople->getWidth() - spaceBetweenPeopleAndText);
			gPeople->setY(y + track * trackHeight);
			gPeople->setHeight(trackHeight / 2);

			gPeople->draw();
		}

		lastTextList[track] = text;

	}

	foreach(PhStripCut * cut, _doc.getCuts())
	{
		if( (cut->getTimeIn() > frameIn) && (cut->getTimeIn() < frameOut))
		{
			PhGraphicRect *gCut = _graphicCuts[cut];
			if(gCut == NULL)
			{
				gCut = new PhGraphicSolidRect();
				gCut->setColor(QColor(0, 0, 0));
				gCut->setZ(-2);

				_graphicCuts[cut] = gCut;
			}
			gCut->setHeight(height);
			gCut->setX(x + cut->getTimeIn() * pixelPerFrame - offset);
			gCut->setY(y);
			gCut->setWidth(2);

			gCut->draw();
			cutCounter++;
		}
	}


	foreach(PhStripLoop * loop, _doc.getLoops())
	{
		if( ((loop->getTimeIn() + height / 8 /pixelPerFrame) > frameIn) && ((loop->getTimeIn() - height / 8 /pixelPerFrame ) < frameOut))
		{
			PhGraphicLoop * gLoop = _graphicLoops[loop];
			if(gLoop == NULL)
			{
				gLoop = new PhGraphicLoop();
				gLoop->setColor(QColor(0, 0, 0, 1));
				_graphicLoops[loop] = gLoop;
			}
			gLoop->setX(x + loop->getTimeIn() * pixelPerFrame - offset);
			gLoop->setY(y);
			gLoop->setHThick(height/40);
			gLoop->setHeight(height);
			gLoop->setCrossHeight(height / 4);
			gLoop->setWidth(height / 4);

			gLoop->draw();
			loopCounter++;
		}
	}

	foreach(PhStripOff * off, _doc.getOffs())
	{
		if( ! (((off->getTimeIn() < frameIn) && (off->getTimeOut() < frameIn)) || ((off->getTimeIn() > frameOut) && (off->getTimeOut() > frameOut))) )
		{
			PhGraphicRect *gOff = _graphicOffs[off];
			if(gOff == NULL)
			{
				gOff = new PhGraphicSolidRect();
				if(off->getPeople())
					gOff->setColor(QColor(off->getPeople()->getColor()));
				gOff->setZ(-2);

				_graphicOffs[off] = gOff;
			}
			gOff->setX(x + off->getTimeIn() * pixelPerFrame - offset);
			gOff->setY(y + off->getTrack() * trackHeight + trackHeight);
			gOff->setHeight( trackHeight / 10);
			gOff->setWidth((off->getTimeOut() - off->getTimeIn()) * pixelPerFrame);
			gOff->draw();
			offCounter++;
		}
	}

//	PHDEBUG << "off counter : " << offCounter << "cut counter : " << cutCounter << "loop counter : " << loopCounter;

}
