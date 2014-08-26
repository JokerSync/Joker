/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"
#include "PhGraphicStrip.h"

PhGraphicStrip::PhGraphicStrip(PhGraphicStripSettings *settings) :
	_settings(settings),
	_maxDrawElapsed(0)
{
	if(!QFile(_settings->backgroundImageDark()).exists())
		_settings->resetBackgroundImageDark();
	if(!QFile(_settings->backgroundImageLight()).exists())
		_settings->resetBackgroundImageLight();
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
	_infos.clear();

	int counter = 0;

	int lastDrawElapsed = _testTimer.elapsed();

	// FIXME
//	if(height > 0) {
//		long delay = (int)(24 * _settings->screenDelay() *  _clock.rate());
//		PhTime clockTime = _clock.time() + delay;

//		if(_settings->stripTestMode()) {
//			QListIterator<PhStripCut*> i = _doc.cutModel()->iterator();
//			while(i.hasNext()) {
//				PhStripCut *cut = i.next();
//				counter++;
//				if(cut->timeIn() == clockTime) {
//					PhGraphicSolidRect white(x, y, width, height);
//					white.setColor(Qt::white);
//					white.draw();

//					//This is useless to continue the foreach if the cut is displayed.
//					break;
//				}
//			}
//			return;
//		}

//		PhTime verticalTimePerPixel = (PhTime)_settings->verticalTimePerPixel();

//		// Change to display the vertical scale via the settings
//		if(_settings->displayVerticalScale()) {
//			PhGraphicSolidRect scale;
//			int scaleHeight = 4;
//			int spaceBetweenDashes = _settings->verticalScaleSpaceInSeconds() * PHTIMEBASE / verticalTimePerPixel;
//			// Set the first dash to +5sec
//			scale.setHeight(scaleHeight);
//			scale.setColor(Qt::gray);
//			int counter = 0;

//			for(int y0 = y - spaceBetweenDashes; y0 > 0; y0 = y0 - spaceBetweenDashes) {
//				int scaleWidth;
//				if(counter % 2 == 0)
//					scaleWidth = 4;
//				else
//					scaleWidth = 8;
//				scale.setWidth(scaleWidth);
//				scale.setPosition(width - scaleWidth, y0, 10 );
//				scale.draw();
//				counter++;
//			}
//		}
//	}

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
