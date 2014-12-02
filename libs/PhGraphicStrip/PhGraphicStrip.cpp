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
#include "PhGraphic/PhGraphicDashedLine.h"
#include "PhGraphic/PhGraphicArrow.h"
#include "PhGraphicStrip.h"

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

	if (_doc.texts().isEmpty())
		return;

	PhNextPeople *nextPeople = new PhNextPeople("",
												"#000000",
												0,
												true,
												_doc.texts().first()->timeIn());
	_nextPeopleModel.addNextPeople(nextPeople);

	for(int i=0; i<_doc.texts().length() - 1; i++)
	{
		PhStripText * text = _doc.texts()[i];
		PhStripText * nextText = _doc.texts()[i+1];
		PhPeople * people = text->people();
		PhNextPeople *nextPeople = new PhNextPeople(people->name(),
													computeColor(people, selectedPeoples, invertedColor).name(),
													text->timeIn(),
													selectedPeoples.size()==0 || selectedPeoples.contains(people),
													nextText->timeIn() - text->timeIn());
		_nextPeopleModel.addNextPeople(nextPeople);
	}

	PhStripText * text = _doc.texts().last();
	PhPeople * people = text->people();
	nextPeople = new PhNextPeople(people->name(),
								computeColor(people, selectedPeoples, invertedColor).name(),
								text->timeIn(),
								selectedPeoples.size()==0 || selectedPeoples.contains(people),
								_doc.timeOut() - text->timeIn());
	_nextPeopleModel.addNextPeople(nextPeople);

	// ruler
	_rulerModel.clear();
	PhTime rulerTimeIn = _settings->rulerTimeIn();
	PhTime timeBetweenRuler = _settings->timeBetweenRuler();
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
		previousTimeOut = 0;
		foreach(PhStripText * text, _doc.texts()) {
			if (text->y() == y) {
				text->setSelected(selectedPeoples.size()==0 || selectedPeoples.contains(text->people()));

				PhStripText *emptyText = new PhStripText(previousTimeOut,
														 text->people(),
														 text->timeIn(),
														 0,
														 "", // empty content
														 0, //height
														 true);

				track->stripTextModel()->addStripText(emptyText);
				track->stripTextModel()->addStripText(text);

				previousTimeOut = text->timeOut();
			}
		}

		// strip text people
		previousTimeOut = 0;
		foreach(PhStripText * text, _doc.texts()) {
			if (text->y() == y) {
				PhStripText *peopleText = new PhStripText(previousTimeOut,
														 text->people(),
														 text->timeIn(),
														 0,
														 text->people()->name(),
														 0, //height
														 selectedPeoples.size()==0 || selectedPeoples.contains(text->people()));

				PhStripText *emptyText = new PhStripText(text->timeIn(),
														  text->people(),
														  text->timeOut(),
														  0,
														  "", //empty content
														  0, //height
														  true);

				track->stripPeopleModel()->addStripText(peopleText);
				track->stripPeopleModel()->addStripText(emptyText);

				previousTimeOut = text->timeOut();
			}
		}

		// off detects
		previousTimeOut = 0;
		foreach(PhStripDetect * detect, _doc.detects()) {
			if (text->y() == y && detect->type() == PhStripDetect::Off) {
				PhNextPeople *detectPeople = new PhNextPeople("", "", previousTimeOut, true, detect->timeIn() - previousTimeOut);
				track->offDetectModel()->addNextPeople(detectPeople);
				previousTimeOut = detect->timeIn();
			}
		}

		// semi-off detects
		previousTimeOut = 0;
		foreach(PhStripDetect * detect, _doc.detects()) {
			if (text->y() == y && detect->type() == PhStripDetect::SemiOff) {
				PhNextPeople *detectPeople = new PhNextPeople("", "", previousTimeOut, true, detect->timeIn() - previousTimeOut);
				track->semiOffDetectModel()->addNextPeople(detectPeople);
				previousTimeOut = detect->timeIn();
			}
		}

		// arrow-up detects
		previousTimeOut = 0;
		foreach(PhStripDetect * detect, _doc.detects()) {
			if (text->y() == y && detect->type() == PhStripDetect::ArrowUp) {
				PhNextPeople *detectPeople = new PhNextPeople("", "", previousTimeOut, true, detect->timeIn() - previousTimeOut);
				track->arrowUpDetectModel()->addNextPeople(detectPeople);
				previousTimeOut = detect->timeIn();
			}
		}

		// arrow-down detects
		previousTimeOut = 0;
		foreach(PhStripDetect * detect, _doc.detects()) {
			if (text->y() == y && detect->type() == PhStripDetect::ArrowDown) {
				PhNextPeople *spacer = new PhNextPeople("spacer", "", previousTimeOut, true, detect->timeIn() - previousTimeOut);
				PhNextPeople *detectPeople = new PhNextPeople("", "", detect->timeIn(), true, detect->timeOut() - detect->timeIn());
				track->arrowDownDetectModel()->addNextPeople(spacer);
				track->arrowDownDetectModel()->addNextPeople(detectPeople);
				previousTimeOut = detect->timeOut();
			}
		}

		_trackModel.addTrack(track);
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
