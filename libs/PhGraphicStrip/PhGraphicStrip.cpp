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
	QList<PhPeople *> selectedPeoples;
	bool invertedColor = _settings->invertColor();

	_nextPeopleModel.clear();

	if (_doc.lineModel()->rowCount() == 0) {
		return;
	}

	QListIterator<PhStripLine*> i = _doc.lineModel()->iterator();
	while (i.hasNext()) {
		PhStripLine *line = i.next();
		PhPeople * people = line->people();
		PhNextPeople *nextPeople = new PhNextPeople(people->name(),
													computeColor(people, selectedPeoples, invertedColor).name(),
													line->timeIn(),
													selectedPeoples.size()==0 || selectedPeoples.contains(people),
													0);
		_nextPeopleModel.addNextPeople(nextPeople);
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

	// cuts
	_cutModel.clear();
	int previousCutTime = 0;
	foreach(PhStripCut * cut, _doc.cuts()) {
		PhNextPeople *cutPeople = new PhNextPeople("", "", previousCutTime, true, cut->timeIn() - previousCutTime);
		_cutModel.addNextPeople(cutPeople);
		previousCutTime = cut->timeIn();
	}

	// loops
	_loopModel.clear();
	int previousLoopTime = 0;
	foreach(PhStripLoop * loop, _doc.loops()) {
		PhNextPeople *loopPeople = new PhNextPeople(loop->label(), "", previousLoopTime, true, loop->timeIn() - previousLoopTime);
		_loopModel.addNextPeople(loopPeople);
		previousLoopTime = loop->timeIn();
	}

	QList<float> trackY;
	trackY << 0 << 0.25 << 0.5 << 0.75;

	foreach(float y, trackY) {
		PhTrack * track = new PhTrack();
		int previousTimeOut;

		// strip texts
//		foreach(PhStripText * text, _doc.texts()) {
//			if (text->y() == y) {
//				text->setSelected(selectedPeoples.size()==0 || selectedPeoples.contains(text->people()));
//				track->stripTextModel()->append(text);
//			}
//		}

		// strip text people
		previousTimeOut = 0;
//		foreach(PhStripText * text, _doc.texts()) {
//			if (text->y() == y) {
//				PhStripText *peopleText = new PhStripText(previousTimeOut,
//														 text->people(),
//														 text->timeIn(),
//														 0,
//														 text->people()->name(),
//														 0, //height
//														 selectedPeoples.size()==0 || selectedPeoples.contains(text->people()));

//				PhStripText *emptyText = new PhStripText(text->timeIn(),
//														  text->people(),
//														  text->timeOut(),
//														  0,
//														  "", //empty content
//														  0, //height
//														  true);

//				track->stripPeopleModel()->append(peopleText);
//				track->stripPeopleModel()->append(emptyText);

//				previousTimeOut = text->timeOut();
//			}
//		}

		// off detects
		previousTimeOut = 0;
		foreach(PhStripDetect * detect, _doc.detects()) {
//			if (text->y() == y && detect->type() == PhStripDetect::Off) {
//				PhNextPeople *detectPeople = new PhNextPeople("", "", previousTimeOut, true, detect->timeIn() - previousTimeOut);
//				track->offDetectModel()->addNextPeople(detectPeople);
//				previousTimeOut = detect->timeIn();
//			}
		}

		// semi-off detects
		previousTimeOut = 0;
		foreach(PhStripDetect * detect, _doc.detects()) {
//			if (text->y() == y && detect->type() == PhStripDetect::SemiOff) {
//				PhNextPeople *detectPeople = new PhNextPeople("", "", previousTimeOut, true, detect->timeIn() - previousTimeOut);
//				track->semiOffDetectModel()->addNextPeople(detectPeople);
//				previousTimeOut = detect->timeIn();
//			}
		}

		// arrow-up detects
		previousTimeOut = 0;
		foreach(PhStripDetect * detect, _doc.detects()) {
//			if (text->y() == y && detect->type() == PhStripDetect::ArrowUp) {
//				PhNextPeople *detectPeople = new PhNextPeople("", "", previousTimeOut, true, detect->timeIn() - previousTimeOut);
//				track->arrowUpDetectModel()->addNextPeople(detectPeople);
//				previousTimeOut = detect->timeIn();
//			}
		}

		// arrow-down detects
		previousTimeOut = 0;
		foreach(PhStripDetect * detect, _doc.detects()) {
//			if (text->y() == y && detect->type() == PhStripDetect::ArrowDown) {
//				PhNextPeople *spacer = new PhNextPeople("spacer", "", previousTimeOut, true, detect->timeIn() - previousTimeOut);
//				PhNextPeople *detectPeople = new PhNextPeople("", "", detect->timeIn(), true, detect->timeOut() - detect->timeIn());
//				track->arrowDownDetectModel()->addNextPeople(spacer);
//				track->arrowDownDetectModel()->addNextPeople(detectPeople);
//				previousTimeOut = detect->timeOut();
//			}
		}
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
