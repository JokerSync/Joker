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

	// strip texts
	_stripTextModelTrack0.clear();
	_stripTextModelTrack1.clear();
	_stripTextModelTrack2.clear();
	_stripTextModelTrack3.clear();
	int previousTimeOutTrack0 = 0;
	int previousTimeOutTrack1 = 0;
	int previousTimeOutTrack2 = 0;
	int previousTimeOutTrack3 = 0;

	foreach(PhStripText * text, _doc.texts()) {
		text->setSelected(selectedPeoples.size()==0 || selectedPeoples.contains(text->people()));

		if (text->y() == 0) {
			PhStripText *emptyText = new PhStripText(previousTimeOutTrack0,
													 text->people(),
													 text->timeIn(),
													 0,
													 "", // empty content
													 0, //height
													 true);

			_stripTextModelTrack0.addStripText(emptyText);
			_stripTextModelTrack0.addStripText(text);

			previousTimeOutTrack0 = text->timeOut();
		}

		if (text->y() == 0.25) {
			PhStripText *emptyText = new PhStripText(previousTimeOutTrack1,
													 text->people(),
													 text->timeIn(),
													 0,
													 "", // empty content
													 0, //height
													 true);

			_stripTextModelTrack1.addStripText(emptyText);
			_stripTextModelTrack1.addStripText(text);

			previousTimeOutTrack1 = text->timeOut();
		}

		if (text->y() == 0.5) {
			PhStripText *emptyText = new PhStripText(previousTimeOutTrack2,
													 text->people(),
													 text->timeIn(),
													 0,
													 "", // empty content
													 0, //height
													 true);

			_stripTextModelTrack2.addStripText(emptyText);
			_stripTextModelTrack2.addStripText(text);

			previousTimeOutTrack2 = text->timeOut();
		}


		if (text->y() == 0.75) {
			PhStripText *emptyText = new PhStripText(previousTimeOutTrack3,
													 text->people(),
													 text->timeIn(),
													 0,
													 "", // empty content
													 0, //height
													 true);

			_stripTextModelTrack3.addStripText(emptyText);
			_stripTextModelTrack3.addStripText(text);

			previousTimeOutTrack3 = text->timeOut();
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
		QMap<float, PhStripText * > lastTextList;


		int verticalTimePerPixel = _settings->verticalTimePerPixel();
		bool displayNextText = _settings->displayNextText();
		PhTime maxTimeIn = timeOut;
		if(displayNextText)
			maxTimeIn += y * verticalTimePerPixel;

		foreach(PhStripText * text, _doc.texts()) {
			PhPeople * people = text->people();
			QString name = people ? people->name() : "???";
			PhGraphicText gPeople(&_hudFont, name);
			gPeople.setWidth(name.length() * 12);
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

			if(displayNextText && ((loop->timeIn() + height * timePerPixel / 8) > timeIn)) {
				PhGraphicLoop gLoopPred;

				int howFarIsLoop = (loop->timeIn() - clockTime) / verticalTimePerPixel;
				gLoopPred.setColor(Qt::white);

				gLoopPred.setHorizontalLoop(true);
				gLoopPred.setZ(-3);

				gLoopPred.setX(width - width / 10);
				gLoopPred.setY(y - howFarIsLoop);
				gLoopPred.setHeight(30);

				gLoopPred.setThickness(3);
				gLoopPred.setCrossSize(20);
				gLoopPred.setWidth(width / 10);

				gLoopPred.draw();
			}
			if((loop->timeIn() - height * timePerPixel / 8) > timeOut + 25 * 30)
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
