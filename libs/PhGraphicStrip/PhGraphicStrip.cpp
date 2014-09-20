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
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicDisc.h"
#include "PhGraphic/PhGraphicDashedLine.h"
#include "PhGraphic/PhGraphicArrow.h"
#include "PhGraphicStrip.h"
#include "PhGraphic/PhGraphicSolidRect.h"
#include "PhGraphic/PhGraphicLoop.h"

PhGraphicStrip::PhGraphicStrip(PhGraphicStripSettings *settings) :
	_settings(settings),
	_maxDrawElapsed(0)
{
	// update the  content when the doc changes :
	this->connect(&_doc, SIGNAL(changed()), this, SLOT(onDocChanged()));

	// This is used to make some time-based test
	_testTimer.start();

}

PhStripDoc *PhGraphicStrip::doc()
{
	return &_doc;
}

PhClock *PhGraphicStrip::clock()
{
	return &_clock;
}

void PhGraphicStrip::onDocChanged()
{

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

void PhGraphicStrip::draw(int x, int y, int width, int height, int nextTextX, int nextTextY, QList<PhPeople *> selectedPeoples)
{
	// Update the resource path if needed
	_backgroundImageLight.setFilename(_settings->backgroundImageLight());
	_backgroundImageDark.setFilename(_settings->backgroundImageDark());
	_backgroundImageLight.setRepeat(true);
	_backgroundImageDark.setRepeat(true);

	_textFont.setFontFile(_settings->textFontFile());
	_textFont.setBoldness(_settings->textBoldness());

	_hudFont.setFontFile(_settings->hudFontFile());

	// Just to preload the font in order to avoid font loading during playback
	_textFont.select();
	_hudFont.select();

	_infos.clear();

	int counter = 0;
	bool invertedColor = _settings->invertColor();

	int lastDrawElapsed = _testTimer.elapsed();
	//PHDEBUG << "time " << _clock.time() << " \trate " << _clock.rate();

	if(height > 0) {
		int timePerPixel = _settings->horizontalTimePerPixel();
		_textFont.setBoldness(_settings->textBoldness());
		_textFont.setFontFile(_settings->textFontFile());

		long syncBar_X_FromLeft = width / 6;
		long delay = (int)(24 * _settings->screenDelay() *  _clock.rate());
		PhTime clockTime = _clock.time() + delay;
		long offset = clockTime / timePerPixel - syncBar_X_FromLeft;

		//Compute the visible duration of the strip
		PhTime stripDuration = width * timePerPixel;


		if(_settings->stripTestMode()) {
			foreach(PhStripCut * cut, _doc.cuts()) {
				counter++;
				if(cut->timeIn() == clockTime) {
					PhGraphicSolidRect white(x, y, width, height);
					white.setColor(Qt::white);
					white.draw();

					//This is useless to continue the foreach if the cut is displayed.
					break;
				}
			}
			return;
		}

		PhTime timeIn = clockTime - syncBar_X_FromLeft * timePerPixel;
		PhTime timeOut = timeIn + stripDuration;


		if(_settings->displayBackground()) {
			//Draw backgroung picture
			int n = width / height + 2; // compute how much background repetition do we need
			long leftBG = 0;
			if(offset >= 0)
				leftBG -= offset % height;
			else
				leftBG -= height - ((-offset) % height);

			PhGraphicTexturedRect* backgroundImage = &_backgroundImageLight;
			if(invertedColor)
				backgroundImage = &_backgroundImageDark;

			backgroundImage->setX(x + leftBG);
			backgroundImage->setY(y);
			backgroundImage->setSize(height * n, height);
			backgroundImage->setZ(-2);
			backgroundImage->setTextureCoordinate(n, 1);
			backgroundImage->draw();
		}
		else {
			PhGraphicSolidRect backgroundRect(x, y, width, height);
			if(_settings->invertColor())
				backgroundRect.setColor(QColor(_settings->backgroundColorDark()));
			else
				backgroundRect.setColor(QColor(_settings->backgroundColorLight()));

			backgroundRect.setZ(-2);
			backgroundRect.draw();
		}

		PhGraphicSolidRect syncBarRect;
		syncBarRect.setColor(QColor(225, 86, 108));
		syncBarRect.setSize(4, height);
		syncBarRect.setPosition(x + width/6, y, 0);

		syncBarRect.draw();

		if(_settings->displayRuler()) {
			PhTime rulerTimeIn = _settings->rulerTimeIn();
			PhTime timeBetweenRuler = _settings->timeBetweenRuler();
			int rulerNumber = (timeIn - rulerTimeIn) / timeBetweenRuler;
			if (rulerNumber < 0)
				rulerNumber = 0;

			PhTime rulerTime = rulerTimeIn + rulerNumber * timeBetweenRuler;
			PhGraphicSolidRect rulerRect;
			PhGraphicDisc rulerDisc;
			PhGraphicText rulerText(&_hudFont);
			QColor rulerColor(80, 80, 80);
			if(invertedColor)
				rulerColor = Qt::white;

			int width = 1000 / timePerPixel;

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


			while (rulerTime < timeOut + timeBetweenRuler) {
				counter++;
				int x = rulerTime / timePerPixel - offset;

				rulerRect.setX(x - rulerRect.width() / 2);
				rulerRect.draw();

				QString text = QString::number(rulerNumber);
				rulerText.setContent(text);
				int textWidth = _hudFont.getNominalWidth(text);
				rulerText.setWidth(textWidth);
				rulerText.setX(x - textWidth / 2);
				rulerText.draw();

				x += timeBetweenRuler / timePerPixel / 2;

				rulerRect.setX(x - rulerRect.width() / 2);
				rulerRect.draw();

				rulerDisc.setX(x);
				rulerDisc.draw();

				rulerNumber++;
				rulerTime += timeBetweenRuler;
			}
		}

		int minTimeBetweenPeople = 48000;
		int timeBetweenPeopleAndText = 4000;
		int spacing = 8;
		QMap<float, PhStripText * > lastTextList;


		int verticalTimePerPixel = _settings->verticalTimePerPixel();
		bool displayNextText = _settings->displayNextText();
		PhTime maxTimeIn = timeOut;

		if(displayNextText)
			maxTimeIn = timeIn + (y - nextTextY) * verticalTimePerPixel;

		QColor selectedPeopleColor(_settings->backgroundColorLight());
		QColor unselectedPeopleColor(128, 128, 128);

		// Display the selected people after the vertical scale
		if(selectedPeoples.count()) {
			QMap<PhTime, PhStripText*> futureSelectedText;
			PhTime maxTimeOut = clockTime + (y - nextTextY) * verticalTimePerPixel;
			foreach (PhPeople *people, selectedPeoples) {
				PhStripText *nextText = _doc.nextText({people}, maxTimeOut);
				if(nextText)
					futureSelectedText[nextText->timeIn() + (int)(10 * nextText->y())] = nextText;
			}
			if(futureSelectedText.count()) {
				QList<PhTime> timeList = futureSelectedText.keys();
				qSort(timeList.begin(), timeList.end());
				foreach(PhTime timeIn, timeList) {
					PhStripText *text = futureSelectedText[timeIn];
					if(text && text->people()) {
						QString name = text->people()->name().toLower();
						PhGraphicText gPeople(&_hudFont, name);
						gPeople.setX(nextTextX + spacing);
						gPeople.setY(nextTextY);
						gPeople.setWidth(_hudFont.getNominalWidth(name) / 2);
						gPeople.setHeight(text->height() * height / 2);

						gPeople.draw();

						nextTextY += gPeople.height();
					}
				}
			}
		}

		// Display the texts
		foreach(PhStripText * text, _doc.texts()) {
			if( !((text->timeOut() < timeIn) || (text->timeIn() > timeOut)) ) {
				counter++;
				PhGraphicText gText(&_textFont, text->content());
				gText.setZ(-1);

				gText.setX(x + text->timeIn() / timePerPixel - offset);
				gText.setWidth((text->timeOut() - text->timeIn()) / timePerPixel);
				gText.setY(y + text->y() * height);
				gText.setHeight(text->height() * height);
				gText.setZ(-1);
				gText.setColor(computeColor(text->people(), selectedPeoples, invertedColor));

				gText.draw();
			}

			PhPeople * people = text->people();
			QString name = people ? people->name().toLower() : "???";
			PhGraphicText gPeople(&_hudFont, name);
			gPeople.setWidth(_hudFont.getNominalWidth(name) / 2);
			gPeople.setHeight(text->height() * height / 2);
			int x0 = x + (text->timeIn() - timeBetweenPeopleAndText) / timePerPixel - offset - gPeople.width();

			PhStripText * lastText = lastTextList[text->y()];
			// Display the people name only if one of the following condition is true:
			// - it is the first text
			// - it is a different people
			// - the distance between the latest text and the current is superior to a limit
			if( x0 < width
			    && x0 + gPeople.width() > 0
			    && (
			        (lastText == NULL)
			        || (lastText->people() != text->people())
			        || (text->timeIn() - lastText->timeOut() > minTimeBetweenPeople))
			    ) {

				gPeople.setX(x0);
				gPeople.setY(y + text->y() * height);
				gPeople.setZ(-1);

				gPeople.setColor(computeColor(people, selectedPeoples, invertedColor));

				gPeople.draw();
			}

			PhTime timePerPeopleHeight = gPeople.height() * verticalTimePerPixel;

			if(displayNextText
			   && (text->timeIn() > clockTime)
			   && (text->timeIn() < maxTimeIn - timePerPeopleHeight)
			   && ((lastText == NULL)
			       || (lastText->people() != text->people())
			       || (text->timeIn() - lastText->timeOut() > minTimeBetweenPeople))) {
				PhPeople * people = text->people();

				//This line is used to see which text's name will be displayed
				gPeople.setX(nextTextX + spacing);
				gPeople.setY(y - (text->timeIn() - clockTime + timePerPeopleHeight) / verticalTimePerPixel);
				gPeople.setZ(-3);
				gPeople.setHeight(height / 10);

				if(selectedPeoples.size() && !selectedPeoples.contains(people))
					gPeople.setColor(unselectedPeopleColor);
				else
					gPeople.setColor(selectedPeopleColor);

				gPeople.draw();
			}

			lastTextList[text->y()] = text;

			if(text->timeIn() > maxTimeIn)
				break;
		}

		if(_settings->displayCuts()) {
			int cutWidth = _settings->cutWidth();
			foreach(PhStripCut * cut, _doc.cuts()) {
				//_counter++;
				if( (timeIn < cut->timeIn()) && (cut->timeIn() < timeOut)) {
					PhGraphicSolidRect gCut;
					gCut.setZ(-1);
					gCut.setWidth(cutWidth);

					if(invertedColor)
						gCut.setColor(QColor(255, 255, 255));
					else
						gCut.setColor(QColor(0, 0, 0));
					gCut.setHeight(height);
					gCut.setX(x + cut->timeIn() / timePerPixel - offset);
					gCut.setY(y);

					gCut.draw();
				}
				//Doesn't need to process undisplayed content
				if(cut->timeIn() > timeOut)
					break;
			}
		}

		foreach(PhStripLoop * loop, _doc.loops()) {
			//_counter++;
			// This calcul allow the cross to come smoothly on the screen (height * timePerPixel / 8)
#warning /// @todo clean this it is not clear
			if( ((loop->timeIn() + height * timePerPixel / 8) > timeIn) && ((loop->timeIn() - height * timePerPixel / 8 ) < timeOut)) {
				PhGraphicLoop gLoop;
				if(!invertedColor)
					gLoop.setColor(Qt::black);
				else
					gLoop.setColor(Qt::white);

				int xLoop = x + loop->timeIn() / timePerPixel - offset;
				gLoop.setX(xLoop);
				gLoop.setY(y);
				gLoop.setZ(-1);
				gLoop.setThickness(height / 40);
				gLoop.setHeight(height);
				gLoop.setCrossSize(height / 4);
				gLoop.setWidth(height / 4);

				gLoop.draw();

				PhGraphicText gLabel(&_hudFont, loop->label(), xLoop + 10, y + height * 3 / 4, -1);
				gLabel.setWidth(_hudFont.getNominalWidth(loop->label()));
				gLabel.setHeight(height / 4);
				gLabel.setColor(Qt::gray);
				gLabel.draw();
			}

			if(displayNextText
					&& (loop->timeIn() > clockTime)
					&& (loop->timeIn() < maxTimeIn)) {
				PhGraphicLoop gLoopPred;

				gLoopPred.setColor(Qt::white);

				gLoopPred.setHorizontalLoop(true);
				gLoopPred.setZ(-3);

				gLoopPred.setX(nextTextX);
				gLoopPred.setY(y - (loop->timeIn() - clockTime) / verticalTimePerPixel);
				gLoopPred.setHeight(30);

				int loopWidth = width - nextTextX;
				gLoopPred.setThickness(4);
				gLoopPred.setCrossSize(loopWidth / 10);
				gLoopPred.setWidth(loopWidth);

				gLoopPred.draw();

				// Display the label
				PhGraphicText gLabel(&_hudFont, loop->label());
				gLabel.setWidth(_hudFont.getNominalWidth(loop->label()) / 3);
#warning /// @todo better loop sizing
				gLabel.setHeight(height / 20);
				gLabel.setX(width - gLabel.width() - spacing);
				gLabel.setY(gLoopPred.y() - gLabel.height() - spacing);
				gLabel.setColor(Qt::gray);
				gLabel.draw();
		}
			if(loop->timeIn() > maxTimeIn + timePerPixel * height / 4)
				break;
		}

		foreach(PhStripDetect * detect, _doc.detects()) {
			//_counter++;

			if((timeIn < detect->timeOut()) && (detect->timeIn() < timeOut) ) {
				PhGraphicRect *gDetect = NULL;
				switch (detect->type()) {
				case PhStripDetect::Off:
					gDetect = new PhGraphicSolidRect();
					gDetect->setY(y + detect->y() * height + detect->height() * height * 0.9);
					gDetect->setHeight(detect->height() * height / 10);
					break;
				case PhStripDetect::SemiOff:
					gDetect = new PhGraphicDashedLine((detect->timeOut() - detect->timeIn()) / 1200);
					gDetect->setY(y + detect->y() * height + detect->height() * height * 0.9);
					gDetect->setHeight(detect->height() * height / 10);
					break;
				case PhStripDetect::ArrowUp:
					gDetect = new PhGraphicArrow(PhGraphicArrow::DownLeftToUpRight);
					gDetect->setY(y + detect->y() * height);
					gDetect->setHeight(detect->height() * height);
					break;
				case PhStripDetect::ArrowDown:
					gDetect = new PhGraphicArrow(PhGraphicArrow::UpLefToDownRight);
					gDetect->setY(y + detect->y() * height);
					gDetect->setHeight(detect->height() * height);
					break;
				default:
					break;
				}

				if(gDetect) {
					gDetect->setColor(computeColor(detect->people(), selectedPeoples, invertedColor));

					gDetect->setX(x + detect->timeIn() / timePerPixel - offset);
					gDetect->setZ(-1);
					gDetect->setWidth((detect->timeOut() - detect->timeIn()) / timePerPixel);
					gDetect->draw();
					delete gDetect;
				}
			}
			//Doesn't need to process undisplayed content
			if(detect->timeIn() > timeOut)
				break;
		}

		// Change to display the ruler via the settings
		if(_settings->displayVerticalScale()) {
			PhGraphicSolidRect scale;
			int scaleHeight = 4;
			int spaceBetweenDashes = _settings->verticalScaleSpaceInSeconds() * 24000 / verticalTimePerPixel;
			// Set the first dash to +5sec
			scale.setHeight(scaleHeight);
			scale.setColor(Qt::gray);
			int counter = 0;

			for(int y0 = y - spaceBetweenDashes; y0 > 0; y0 = y0 - spaceBetweenDashes) {
				int scaleWidth;
				if(counter % 2 == 0)
					scaleWidth = 4;
				else
					scaleWidth = 8;
				scale.setWidth(scaleWidth);
				scale.setPosition(width - scaleWidth, y0, 10 );
				scale.draw();
				counter++;
			}
		}
	}

	//	PHDEBUG << "off counter : " << offCounter << "cut counter : " << cutCounter << "loop counter : " << loopCounter;

	int currentDrawElapsed = _testTimer.elapsed() - lastDrawElapsed;
	if(currentDrawElapsed > _maxDrawElapsed)
		_maxDrawElapsed = currentDrawElapsed;
	_testTimer.restart();

	_infos.append(QString("Max strip draw: %1").arg(_maxDrawElapsed));
	_infos.append(QString("Count: %1").arg(counter));

	if(_settings->resetInfo())
		_maxDrawElapsed = 0;
}
