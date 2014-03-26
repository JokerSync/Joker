/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QFile>
#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>

#include "PhTools/PhDebug.h"
#include "PhGraphic/PhGraphicDisc.h"
#include "PhGraphicStrip.h"

PhGraphicStrip::PhGraphicStrip(QObject *parent) :
	QObject(parent),
	_doc(this),
	_clock(_doc.timeCodeType()),
	_trackNumber(4),
	_settings(NULL),
	_maxDrawElapsed(0),
	_dropDetected(0)
{
	// update the  content when the doc changes :
	this->connect(&_doc, SIGNAL(changed()), this, SLOT(onDocChanged()));
}

PhStripDoc *PhGraphicStrip::doc()
{
	return &_doc;
}

PhClock *PhGraphicStrip::clock()
{
	return &_clock;
}

void PhGraphicStrip::setSettings(PhGraphicStripSettings *settings)
{
	PHDEBUG;
	_settings = settings;
}

bool PhGraphicStrip::setFontFile(QString fontFile)
{
	if(_textFont.setFontFile(fontFile)) {
		if(_settings)
			_settings->setTextFontFile(fontFile);
		return true;
	}
	return false;
}

bool PhGraphicStrip::init()
{
	PHDEBUG << _settings;

	PHDEBUG << "Load the strip background";
	_stripBackgroundImage.setFilename(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/motif-240.png");
	_stripBackgroundImage.init();

	_stripBackgroundImageInverted.setFilename(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/motif-240_black.png");
	_stripBackgroundImageInverted.init();

	PHDEBUG << "Init the sync bar";
	_stripSyncBar.setColor(QColor(225, 86, 108));

	PHDEBUG << "Load the font file";
	QString fontFile = "";
	if(_settings != NULL)
		fontFile = _settings->textFontFile();
	else
		PHDEBUG << "no settings...";

	if(!QFile(fontFile).exists()) {
		PHDEBUG << "File not found:" << fontFile;
		fontFile = QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/" + "SWENSON.TTF";
		if(_settings != NULL)
			_settings->textFontFile();
		else
			PHDEBUG << "no settings...";
	}
	_textFont.setFontFile(fontFile);

	if(_settings != NULL)
		_textFont.setBoldness(_settings->textBoldness());

	// Init the sync bar
	_stripSyncBar.setColor(QColor(225, 86, 108));

	_hudFont.setFontFile(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/" + "ARIAL.TTF");

	// This is used to make some time-based test
	_testTimer.start();

	return true;
}

void PhGraphicStrip::onDocChanged()
{
	qDeleteAll(_graphicPeoples);
	_graphicPeoples.clear();
	foreach(PhPeople *people, _doc.peoples()) {
		QString name = people ? people->name() : "???";
		_graphicPeoples[people] = new PhGraphicText(&_hudFont, name);
		_graphicPeoples[people]->setWidth(name.length() * 12);
		_graphicPeoples[people]->init();
	}

	qDeleteAll(_graphicCuts);
	_graphicCuts.clear();
	foreach(PhStripCut *cut, _doc.cuts()) {
		_graphicCuts[cut] = new PhGraphicSolidRect();
		_graphicCuts[cut]->setZ(-1);
		_graphicCuts[cut]->setWidth(2);
		_graphicCuts[cut]->init();
	}

	qDeleteAll(_graphicTexts);
	_graphicTexts.clear();
	_trackNumber = 4;
	foreach(PhStripText *text, _doc.texts()) {
		_graphicTexts[text] = new PhGraphicText(&_textFont, text->content());
		_graphicTexts[text]->setZ(-1);
		_graphicTexts[text]->init();
		if(text->track() >= _trackNumber)
			_trackNumber = text->track() + 1;
	}

	qDeleteAll(_graphicLoops);
	_graphicLoops.clear();
	foreach(PhStripLoop *loop, _doc.loops()) {
		_graphicLoops[loop] = new PhGraphicLoop();
		_graphicLoops[loop]->setZ(-1);
		_graphicLoops[loop]->init();
	}

	qDeleteAll(_graphicDetects);
	_graphicDetects.clear();
	foreach(PhStripDetect *detect, _doc.detects()) {
		if(detect->off()) {
			_graphicDetects[detect] = new PhGraphicSolidRect();
			_graphicDetects[detect]->setZ(-1);
			_graphicDetects[detect]->init();
		}
		if(detect->track() >= _trackNumber)
			_trackNumber = detect->track() + 1;
	}
}

PhFont *PhGraphicStrip::getTextFont()
{
	return &_textFont;
}

PhFont *PhGraphicStrip::getHUDFont()
{
	return &_hudFont;
}

QColor PhGraphicStrip::computeColor(PhPeople * people, QList<PhPeople*> selectedPeoples, bool invertColor)
{
	if(!invertColor) {
		if(people) {
			if(selectedPeoples.size() && !selectedPeoples.contains(people)) {
				return QColor(100, 100, 100);
			}
			else {
				return people->color();
			}
		}
		else if(selectedPeoples.size())
			return QColor(100, 100, 100);
		else
			return Qt::black;
	}
	else {
		if(people) {
			if(selectedPeoples.size() && !selectedPeoples.contains(people)) {
				return QColor(155, 155, 155);
			}
			else {
				QColor color(people->color());
				return QColor(255 - color.red(), 255 - color.green(), 255 - color.blue());
			}
		}
		else if(selectedPeoples.size())
			return QColor(155, 155, 155);
		else
			return Qt::white;
	}
}

void PhGraphicStrip::draw(int x, int y, int width, int height, int tcOffset, QList<PhPeople *> selectedPeoples)
{
	int counter = 0;
	bool invertedColor = _settings->invertColor();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	int lastDrawElapsed = _testTimer.elapsed();
	//PHDEBUG << "time " << _clock.time() << " \trate " << _clock.rate();

	if(height > 0) {
		int pixelPerFrame = _settings->horizontalSpeed();
		_textFont.setBoldness(_settings->textBoldness());
		_textFont.setFontFile(_settings->textFontFile());

		int loopCounter = 0;
		int offCounter = 0;
		int cutCounter = 0;

		int fps = PhTimeCode::getFps(_clock.timeCodeType());
		long syncBar_X_FromLeft = width / 6;
		long offset = _clock.time() * pixelPerFrame * fps / _clock.timeScale() - syncBar_X_FromLeft;
		long delay = (int)(_settings->screenDelay() * _clock.rate()); // delay in ms
		// add the delay to the offset
		offset += delay * pixelPerFrame * fps / 1000;
		//Compute the visible duration of the strip
		PhFrame stripDuration = width / pixelPerFrame;

		PhFrame clockFrame = _clock.frame() + delay * fps / 1000;

		if(_settings->stripTestMode()) {
			foreach(PhStripCut * cut, _doc.cuts())
			{
				counter++;
				if(cut->frameIn() == clockFrame) {
					PhGraphicSolidRect white(x, y, width, height);
					white.setColor(Qt::white);
					white.draw();
					PHDEBUG << clockFrame << "cut" << _clock.frame();

					//This is useless to continue the foreach if the cut is displayed.
					break;
				}
			}
			return;
		}

		PhFrame frameIn = clockFrame - syncBar_X_FromLeft / pixelPerFrame;
		PhFrame frameOut = frameIn + stripDuration;

		//Draw backgroung picture
		int n = width / height + 2; // compute how much background repetition do we need
		long leftBG = 0;
		if(offset >= 0)
			leftBG -= offset % height;
		else
			leftBG -= height - ((-offset) % height);

		PhGraphicTexturedRect* backgroundImage = &_stripBackgroundImage;
		if(invertedColor)
			backgroundImage = &_stripBackgroundImageInverted;

		backgroundImage->setX(x + leftBG);
		backgroundImage->setY(y);
		backgroundImage->setSize(height * n, height);
		backgroundImage->setZ(-2);
		backgroundImage->setTextureCoordinate(n, 1);
		backgroundImage->draw();

		_stripSyncBar.setSize(4, height);
		_stripSyncBar.setPosition(x + width/6, y, 0);
		_stripSyncBar.setColor(QColor(225, 86, 108));

		_stripSyncBar.draw();

		if(_settings->displayRuler()) {
			PhFrame rulerTimestamp = _settings->rulerTimestamp();
			PhFrame spaceBetweenRuler = _settings->spaceBetweenRuler();
			int rulerNumber = (frameIn - rulerTimestamp) / spaceBetweenRuler;
			if (rulerNumber < 0)
				rulerNumber = 0;

			PhFrame rulerFrame = rulerTimestamp + rulerNumber * spaceBetweenRuler;
			PhGraphicSolidRect rulerRect;
			PhGraphicDisc rulerDisc;
			PhGraphicText rulerText(&_hudFont);
			QColor rulerColor(80, 80, 80);
			if(invertedColor)
				rulerColor = Qt::white;

			int width = pixelPerFrame;

			rulerRect.setColor(rulerColor);
			rulerRect.setWidth(width);
			rulerRect.setHeight(height / 2);
			rulerRect.setZ(0);
			rulerRect.setY(y);

			rulerDisc.setColor(rulerColor);
			rulerDisc.setRadius(2 * width);
			rulerDisc.setY(y + height / 2 + 3 * width);
			rulerDisc.setZ(0);

			rulerText.setColor(rulerColor);
			rulerText.setY(y + height / 2);
			rulerText.setHeight(height / 2);
			rulerText.setZ(0);


			while (rulerFrame < frameOut + spaceBetweenRuler) {
				counter++;
				int x = rulerFrame * pixelPerFrame - offset;

				rulerRect.setX(x - rulerRect.getWidth() / 2);
				rulerRect.draw();

				QString text = QString::number(rulerNumber);
				rulerText.setContent(text);
				int textWidth = _hudFont.getNominalWidth(text);
				rulerText.setWidth(textWidth);
				rulerText.setX(x - textWidth / 2);
				rulerText.draw();

				x += spaceBetweenRuler * pixelPerFrame / 2;

				rulerRect.setX(x - rulerRect.getWidth() / 2);
				rulerRect.draw();

				rulerDisc.setX(x);
				rulerDisc.draw();

				rulerNumber++;
				rulerFrame += spaceBetweenRuler;
			}
		}

		int minSpaceBetweenPeople = 50;
		int spaceBetweenPeopleAndText = 4;
		PhStripText ** lastTextList = new PhStripText*[_trackNumber];
		for(int i = 0; i < _trackNumber; i++)
			lastTextList[i] = NULL;

		int trackHeight = height / _trackNumber;

		bool trackFull[_trackNumber];
		for(int i = 0; i < _trackNumber; i++) {
			trackFull[i] = false;
		}

		int verticalPixelPerFrame = _settings->verticalSpeed();
		bool displayNextText = _settings->displayNextText();
		PhFrame maxFrameIn = frameOut;
		if(displayNextText)
			maxFrameIn += y / verticalPixelPerFrame;

		foreach(PhStripText * text, _doc.texts())
		{
			counter++;
			PhGraphicText* gText = _graphicTexts[text];
			int track = text->track();

			if( !((text->frameOut() < frameIn) || (text->frameIn() > frameOut)) ) {
				gText->setX(x + text->frameIn() * pixelPerFrame - offset);
				gText->setWidth((text->frameOut() - text->frameIn()) * pixelPerFrame);
				gText->setY(y + track * trackHeight);
				gText->setHeight(trackHeight);
				gText->setZ(-1);
				gText->setColor(computeColor(text->people(), selectedPeoples, invertedColor));

				gText->draw();
			}

			PhStripText * lastText = lastTextList[track];
			// Display the people name only if one of the following condition is true:
			// - it is the first text
			// - it is a different people
			// - the distance between the latest text and the current is superior to a limit
			if((lastText == NULL) || (lastText->people() != text->people()) || (text->frameIn() - lastText->frameOut() > minSpaceBetweenPeople)) {
				PhPeople * people = text->people();
				PhGraphicText * gPeople = _graphicPeoples[people];

				gPeople->setX(x + text->frameIn() * pixelPerFrame - offset - gPeople->getWidth() - spaceBetweenPeopleAndText);
				gPeople->setY(y + track * trackHeight);
				gPeople->setZ(-1);
				gPeople->setHeight(trackHeight / 2);

				gPeople->setColor(computeColor(people, selectedPeoples, invertedColor));

				gPeople->draw();

				//Check if the name is printed on the screen
				if( (frameIn < text->frameOut()) && (text->frameIn() - gPeople->getWidth() / pixelPerFrame < frameOut) ) {
					trackFull[track] = true;
				}

			}

			if(displayNextText && (frameIn < text->frameIn()) && ((lastText == NULL) || (text->frameIn() - lastText->frameOut() > minSpaceBetweenPeople))) {
				PhPeople * people = text->people();

				PhGraphicText * gPeople = _graphicPeoples[people];
				int howFarIsText = (text->frameIn() - frameOut) * verticalPixelPerFrame;
				//This line is used to see which text's name will be displayed
				gPeople->setX(width - gPeople->getWidth());
				gPeople->setY(y - howFarIsText - gPeople->getHeight());

				gPeople->setZ(-3);
				gPeople->setHeight(trackHeight / 2);

				gPeople->setColor(computeColor(people, selectedPeoples, invertedColor));


				PhGraphicSolidRect background(gPeople->getX(), gPeople->getY(), gPeople->getWidth(), gPeople->getHeight() + 2);
				if(selectedPeoples.size() && !selectedPeoples.contains(people))
					background.setColor(QColor(90, 90, 90));
				else
					background.setColor(QColor(180, 180, 180));

				background.setZ(gPeople->getZ() - 1);

				if(gPeople->getY() > tcOffset) {
					if(!invertedColor)
						background.draw();

					gPeople->draw();
				}
			}

			lastTextList[track] = text;

			if(text->frameIn() > maxFrameIn)
				break;
		}

		delete lastTextList;

		foreach(PhStripCut * cut, _doc.cuts())
		{
			//_counter++;
			if( (frameIn < cut->frameIn()) && (cut->frameIn() < frameOut)) {
				PhGraphicSolidRect *gCut = _graphicCuts[cut];
				if(invertedColor)
					gCut->setColor(QColor(255, 255, 255));
				else
					gCut->setColor(QColor(0, 0, 0));
				gCut->setHeight(height);
				gCut->setX(x + cut->frameIn() * pixelPerFrame - offset);
				gCut->setY(y);

				gCut->draw();
				cutCounter++;
			}
			//Doesn't need to process undisplayed content
			if(cut->frameIn() > frameOut)
				break;
		}

		foreach(PhStripLoop * loop, _doc.loops())
		{
			//_counter++;
			// This calcul allow the cross to come smoothly on the screen (height / 8 /pixelPerFrame)
			if( ((loop->frameIn() + height / 8 /pixelPerFrame) > frameIn) && ((loop->frameIn() - height / 8 /pixelPerFrame ) < frameOut)) {
				PhGraphicLoop * gLoop = _graphicLoops[loop];
				if(!invertedColor)
					gLoop->setColor(Qt::black);
				else
					gLoop->setColor(Qt::white);

				gLoop->setX(x + loop->frameIn() * pixelPerFrame - offset);
				gLoop->setY(y);
				gLoop->setHThick(height / 40);
				gLoop->setHeight(height);
				gLoop->setCrossHeight(height / 4);
				gLoop->setWidth(height / 4);

				gLoop->draw();
				loopCounter++;
			}

			if(displayNextText && ((loop->frameIn() + height / 8 / pixelPerFrame) > frameIn)) {
				PhGraphicLoop gLoopPred;

				int howFarIsLoop = (loop->frameIn() - frameOut) * verticalPixelPerFrame;
				gLoopPred.setColor(Qt::white);

				gLoopPred.setHorizontalLoop(true);
				gLoopPred.setZ(-3);

				gLoopPred.setX(width - width / 10);
				gLoopPred.setY(y - howFarIsLoop);
				gLoopPred.setHeight(30);

				gLoopPred.setHThick(3);
				gLoopPred.setCrossHeight(20);
				gLoopPred.setWidth(width / 10);

				gLoopPred.draw();
			}
			if((loop->frameIn() - height / 8 / pixelPerFrame ) > frameOut + 25 * 30)
				break;
		}

		foreach(PhStripDetect * detect, _doc.detects())
		{
			//_counter++;

			if( detect->off() && (frameIn < detect->frameOut()) && (detect->frameIn() < frameOut) ) {
				PhGraphicSolidRect *gDetect = _graphicDetects[detect];

				gDetect->setColor(computeColor(detect->people(), selectedPeoples, invertedColor));

				gDetect->setX(x + detect->frameIn() * pixelPerFrame - offset);
				gDetect->setY(y + detect->track() * trackHeight + trackHeight * 0.8);
				gDetect->setHeight(trackHeight / 20);
				gDetect->setWidth((detect->frameOut() - detect->frameIn()) * pixelPerFrame);
				gDetect->draw();
				offCounter++;
			}
			//Doesn't need to process undisplayed content
			if(detect->frameIn() > frameOut)
				break;
		}
	}

	//	PHDEBUG << "off counter : " << offCounter << "cut counter : " << cutCounter << "loop counter : " << loopCounter;

	int currentDrawElapsed = _testTimer.elapsed() - lastDrawElapsed;
	if(_testTimer.elapsed() > 20)
		PHDEBUG << "Drop detected:" << ++_dropDetected << currentDrawElapsed;
	if(currentDrawElapsed > _maxDrawElapsed)
		_maxDrawElapsed = currentDrawElapsed;
	_testTimer.restart();

	if(_settings->displayStripInfo()) {
		int inc = 60;
		PhGraphicText text(&_hudFont, "", 0, 0, 200, inc);
		text.setColor(Qt::red);
		text.setContent(QString("Drop : %1").arg(_dropDetected));
		text.draw();
		text.setY(text.getY() + inc);
		text.setContent(QString("Max : %1").arg(_maxDrawElapsed));
		text.draw();
		text.setY(text.getY() + inc);
		text.setContent(QString("Count : %1").arg(counter));
		text.draw();
		text.setY(text.getY() + inc);
		text.setContent(QString("people : %1").arg(_graphicPeoples.count()));
		text.draw();
		text.setY(text.getY() + inc);
		text.setContent(QString("text : %1").arg(_graphicTexts.count()));
		text.draw();
	}
}
