/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhFile.h"
#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhUI.h"
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
	if (_doc.lineModel()->rowCount() == 0) {
		return;
	}

	// ruler
	_rulerModel.clear();
	PhTime rulerTimeIn = _settings->firstFootTime();
	PhTime timeBetweenRuler = _settings->timeBetweenTwoFeet();
	PhTime rulerTime = rulerTimeIn;
	PhTime docTimeOut = _doc.timeOut();
	while (rulerTime < docTimeOut + timeBetweenRuler) {
		int rulerNumber = (rulerTime - rulerTimeIn) / timeBetweenRuler;
		PhNextPeople *rulerPeople = new PhNextPeople(QString::number(rulerNumber), "", rulerTime, true, timeBetweenRuler);
		_rulerModel.addNextPeople(rulerPeople);
		rulerTime += timeBetweenRuler;
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

	int lastDrawElapsed = _testTimer.elapsed();

	if(height > 0) {
		_textFont.setBoldness(_settings->textBoldness());
		_textFont.setFontFile(_settings->textFontFile());

		long delay = (int)(24 * _settings->screenDelay() *  _clock.rate());
		PhTime clockTime = _clock.time() + delay;

		if(_settings->stripTestMode()) {
			QListIterator<PhStripCut*> i = _doc.cutModel()->iterator();
			while(i.hasNext()) {
				PhStripCut *cut = i.next();
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

//		if(_settings->displayBackground()) {
//			//Draw backgroung picture
//			int n = width / height + 2; // compute how much background repetition do we need
//			long leftBG = 0;
//			if(offset >= 0)
//				leftBG -= offset % height;
//			else
//				leftBG -= height - ((-offset) % height);

//			PhGraphicTexturedRect* backgroundImage = &_backgroundImageLight;
//			if(_settings->invertColor())
//				backgroundImage = &_backgroundImageDark;

//			backgroundImage->setX(x + leftBG);
//			backgroundImage->setY(y);
//			backgroundImage->setSize(height * n, height);
//			backgroundImage->setZ(-2);
//			backgroundImage->setTextureCoordinate(n, 1);
//			backgroundImage->draw();
//		}
//		else {
//			PhGraphicSolidRect backgroundRect(x, y, width, height);
//			if(_settings->invertColor())
//				backgroundRect.setColor(QColor(_settings->backgroundColorDark()));
//			else
//				backgroundRect.setColor(QColor(_settings->backgroundColorLight()));

//			backgroundRect.setZ(-2);
//			backgroundRect.draw();
//		}

		int verticalTimePerPixel = _settings->verticalTimePerPixel();

		// Change to display the vertical scale via the settings
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
	_infos.append(QString(" Count: %1").arg(counter));

	if(_settings->resetInfo())
		_maxDrawElapsed = 0;
}
