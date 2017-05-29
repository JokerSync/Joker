/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicDisc.h"
#include "PhGraphic/PhGraphicDashedLine.h"
#include "PhGraphic/PhGraphicArrow.h"
#include "PhGraphicStrip.h"
#include "PhGraphic/PhGraphicSolidRect.h"

#include "PhGraphicLoop.h"

PhGraphicStrip::PhGraphicStrip(PhGraphicStripSettings *settings) :
	_settings(settings),
	_maxDrawElapsed(0)
{
	if(!QFile(_settings->backgroundImageDark()).exists())
		_settings->resetBackgroundImageDark();
	if(!QFile(_settings->backgroundImageLight()).exists())
		_settings->resetBackgroundImageLight();
	// This is used to make some time-based test
	_hudFont.setFamily(_settings->hudFontFamily());
	_textFont.setFamily(_settings->textFontFamily());
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

PhFont *PhGraphicStrip::getTextFont()
{
	return &_textFont;
}

PhFont *PhGraphicStrip::hudFont()
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

void PhGraphicStrip::draw(int x, int y, int width, int height, int nextTextAreaX, int nextTextAreaY, QList<PhPeople *> selectedPeoples)
{
	// Update the resource path if needed
	_backgroundImageLight.setFilename(_settings->backgroundImageLight());
	_backgroundImageDark.setFilename(_settings->backgroundImageDark());
	_backgroundImageLight.setRepeat(true);
	_backgroundImageDark.setRepeat(true);

	_textFont.setFamily(_settings->textFontFamily());

	_hudFont.setFamily(_settings->hudFontFamily());

	// Just to preload the font in order to avoid font loading during playback
	_textFont.select();
	_hudFont.select();

	_infos.clear();

	int counter = 0;
	bool invertedColor = _settings->invertColor();

	int lastDrawElapsed = _testTimer.elapsed();
	//PHDEBUG << "time " << _clock.time() << " \trate " << _clock.rate();

	if(height > 0) {
		PhTime timePerPixel = (PhTime)_settings->horizontalTimePerPixel();

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

		PhTime stripTimeIn = clockTime - syncBar_X_FromLeft * timePerPixel;
		PhTime stripTimeOut = stripTimeIn + stripDuration;


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

		if(_settings->displayFeet()) {
			PhTime firstFootTime = _settings->firstFootTime();
			PhTime timeBetweenTwoFeet = _settings->timeBetweenTwoFeet();
			int feetNumber = (stripTimeIn - firstFootTime) / timeBetweenTwoFeet;
			if (feetNumber < 0)
				feetNumber = 0;

			PhTime footTime = firstFootTime + feetNumber * timeBetweenTwoFeet;
			PhGraphicSolidRect footRect;
			PhGraphicDisc footDisc;
			PhGraphicText footText(&_hudFont);
			QColor footColor(80, 80, 80);
			if(invertedColor)
				footColor = Qt::white;

			int width = 250 / timePerPixel;

			footRect.setColor(footColor);
			footRect.setWidth(width);
			footRect.setHeight(height / 2);
			footRect.setZ(0);
			footRect.setY(y);

			footDisc.setColor(footColor);
			footDisc.setRadius(2 * width);
			footDisc.setY(y + height / 2 + 3 * width);
			footDisc.setZ(0);

			footText.setColor(footColor);
			footText.setY(y + height / 2);
			footText.setHeight(height / 2);
			footText.setZ(0);

			while (footTime < stripTimeOut + timeBetweenTwoFeet) {
				counter++;
				int x = footTime / timePerPixel - offset;

				footRect.setX(x - footRect.width() / 2);
				footRect.draw();

				QString text = QString::number(feetNumber);
				footText.setContent(text);
				int textWidth = _hudFont.getNominalWidth(text) / 3;
				footText.setWidth(textWidth);
				footText.setX(x - textWidth / 2);
				footText.draw();

				x += timeBetweenTwoFeet / timePerPixel / 2;

				footRect.setX(x - footRect.width() / 2);
				footRect.draw();

				footDisc.setX(x);
				footDisc.draw();

				feetNumber++;
				footTime += timeBetweenTwoFeet;
			}
		}

		int minTimeBetweenPeople = 48000;
		int timeBetweenPeopleAndText = 4000;
		int spacing = 8;
		QMap<float, PhStripText * > lastTextList;


		PhTime verticalTimePerPixel = (PhTime)_settings->verticalTimePerPixel();
		bool displayNextText = _settings->displayNextText();
		PhTime maxTimeIn = stripTimeOut;

		int nextPeopleHeight = width / 60;
		PhTime timePerPeopleHeight = nextPeopleHeight * verticalTimePerPixel;

		PhTime verticalScaleDuration = (y - nextTextAreaY) * verticalTimePerPixel;
		if(!_settings->hideSelectedPeoples() && selectedPeoples.count())
			verticalScaleDuration -= selectedPeoples.count() * timePerPeopleHeight;
		else
			verticalScaleDuration -= timePerPeopleHeight;

		if(displayNextText && (clockTime + verticalScaleDuration > stripTimeOut))
			maxTimeIn = clockTime + verticalScaleDuration;

		QColor selectedPeopleColor(_settings->backgroundColorLight());
		QColor unselectedPeopleColor(selectedPeopleColor.red() / 2, selectedPeopleColor.green() / 2, selectedPeopleColor.blue() / 2);

		// Display the selected people after the vertical scale
		if(displayNextText && !_settings->hideSelectedPeoples() && selectedPeoples.count()) {
			// Compute the next time in for each people
			QMap<PhTime, PhStripText*> futureSelectedTexts;
			QList<PhPeople*> notAppearingPeoples;
			foreach (PhPeople *people, selectedPeoples) {
				PhStripText *nextText = _doc.nextText(people, maxTimeIn);
				if(nextText)
					futureSelectedTexts[nextText->timeIn()] = nextText;
				else
					notAppearingPeoples.append(people);
			}
			PhGraphicText gPeople(&_hudFont);
			gPeople.setX(nextTextAreaX + spacing);
			gPeople.setHeight(nextPeopleHeight);
			foreach(PhPeople *people, notAppearingPeoples) {
				QString name = people->name().toLower();
				gPeople.setContent(name);
				gPeople.setColor(unselectedPeopleColor);
				gPeople.setY(nextTextAreaY);
				gPeople.setWidth(_hudFont.getNominalWidth(name) * nextPeopleHeight / 110);

				gPeople.draw();

				nextTextAreaY += nextPeopleHeight;
			}

			if(futureSelectedTexts.count()) {
				// Sort so that the next people of the list appear at the bottom of the list
				QList<PhTime> timeList = futureSelectedTexts.keys();
				qSort(timeList.begin(), timeList.end());
				for(int i = timeList.size() - 1; i >= 0; i--) {
					PhTime timeIn = timeList[i];
					PhStripText *text = futureSelectedTexts[timeIn];
					if(text && text->people()) {
						QString name = text->people()->name().toLower();
						gPeople.setContent(name);
						gPeople.setColor(selectedPeopleColor);
						gPeople.setY(nextTextAreaY);
						gPeople.setWidth(_hudFont.getNominalWidth(name) * nextPeopleHeight / 110);

						gPeople.draw();

						nextTextAreaY += nextPeopleHeight;

					}
				}
			}
		}

		int lastNextTextY = std::numeric_limits<int>::max();
		// Display the texts
		foreach(PhStripText * text, _doc.texts()) {
			if( !((text->timeOut() < stripTimeIn) || (text->timeIn() > stripTimeOut)) ) {
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
			gPeople.setWidth(_hudFont.getNominalWidth(name) / 5);
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

			if(displayNextText
			   && (text->timeIn() > clockTime)
			   && (text->timeIn() < maxTimeIn)
			   && ((lastText == NULL)
			       || (lastText->people() != text->people())
			       || (text->timeIn() - lastText->timeOut() > minTimeBetweenPeople))) {
				PhPeople * people = text->people();
				int nextTextX = nextTextAreaX + spacing;
				int nextTextY = y - (text->timeIn() - clockTime) / verticalTimePerPixel - nextPeopleHeight;

				if (nextTextY > lastNextTextY - nextPeopleHeight) {
					nextTextY = lastNextTextY - nextPeopleHeight;
				}
				lastNextTextY = nextTextY;

				gPeople.setX(nextTextX);
				gPeople.setY(nextTextY);
				gPeople.setHeight(nextPeopleHeight);
				gPeople.setWidth(_hudFont.getNominalWidth(name) * nextPeopleHeight / 110);

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
				if( (stripTimeIn < cut->timeIn()) && (cut->timeIn() < stripTimeOut)) {
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
				if(cut->timeIn() > stripTimeOut)
					break;
			}
		}

		foreach(PhStripLoop * loop, _doc.loops()) {
			//_counter++;
			// This calcul allow the cross to come smoothly on the screen (height * timePerPixel / 8)
#warning /// @todo clean this it is not clear
			if( ((loop->timeIn() + height * timePerPixel / 8) > stripTimeIn) && ((loop->timeIn() - height * timePerPixel / 8 ) < stripTimeOut)) {
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

				PhGraphicText gLabel(&_hudFont, loop->label(), xLoop + 10, y + height * 4 / 5, -1);
				gLabel.setWidth(_hudFont.getNominalWidth(loop->label()) / 2);
				gLabel.setHeight(height / 5);
				gLabel.setColor(Qt::gray);
				gLabel.draw();
			}

			if(displayNextText
			   && (loop->timeIn() > clockTime)
			   && (loop->timeIn() < maxTimeIn)) {
				PhGraphicSolidRect loopDash;

				loopDash.setColor(unselectedPeopleColor);

				loopDash.setZ(-3);

				loopDash.setX(nextTextAreaX);
				loopDash.setY(y - (loop->timeIn() - clockTime) / verticalTimePerPixel);
				loopDash.setHeight(4);

				int loopWidth = width - nextTextAreaX;
				loopDash.setWidth(loopWidth);

				loopDash.draw();

				// Display the label
				PhGraphicText gLabel(&_hudFont, loop->label());
				gLabel.setHeight(nextPeopleHeight);
				gLabel.setWidth(_hudFont.getNominalWidth(loop->label()) * nextPeopleHeight / 110);
#warning /// @todo better loop sizing
				gLabel.setX(width - gLabel.width() - spacing);
				gLabel.setY(loopDash.y() - gLabel.height() - spacing);
				gLabel.setColor(unselectedPeopleColor);
				gLabel.draw();
			}
			if(loop->timeIn() > maxTimeIn + timePerPixel * height / 4)
				break;
		}

		foreach(PhStripDetect * detect, _doc.detects()) {
			//_counter++;

			if((stripTimeIn < detect->timeOut()) && (detect->timeIn() < stripTimeOut) ) {
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
			if(detect->timeIn() > stripTimeOut)
				break;
		}

		// Change to display the vertical scale via the settings
		if(_settings->displayVerticalScale()) {
			PhGraphicSolidRect scale;
			int scaleHeight = 4;
			int spaceBetweenDashes = _settings->verticalScaleSpaceInSeconds() * PHTIMEBASE / verticalTimePerPixel;
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
