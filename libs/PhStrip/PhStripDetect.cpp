/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStripDetect.h"

PhStripDetect::PhStripDetect(PhDetectType type, PhTime timeIn, PhPeople *people, PhTime timeOut, float y, float height)
	: PhStripPeopleObject(timeIn, people, timeOut, y, height),
	_type(type)
{

}

QString PhStripDetect::description(PhTimeCodeType tcType)
{
	QString typeString = "";
	switch (this->type()) {
	case Unknown:
		typeString = "Unknown";
		break;
	case On:
		typeString = "On";
		break;
	case SemiOff:
		typeString = "SemiOff";
		break;
	case Off:
		typeString = "Off";
		break;
	case MouthOpen:
		typeString = "MouthOpen";
		break;
	case MouthClosed:
		typeString = "MouthClosed";
		break;
	case Aperture:
		typeString = "Aperture";
		break;
	case Advance:
		typeString = "Advance";
		break;
	case Labial:
		typeString = "Labial";
		break;
	case SemiLabial:
		typeString = "SemiLabial";
		break;
	case Bowl:
		typeString = "Bowl";
		break;
	case Dental:
		typeString = "Dental";
		break;
	case ArrowUp:
		typeString = "ArrowUp";
		break;
	case ArrowDown:
		typeString = "ArrowDown";
		break;
	case AmbianceStart:
		typeString = "AmbianceStart";
		break;
	case AmbianceEnd:
		typeString = "AmbianceEnd";
		break;
	}

	return this->peopleObjectDescription(tcType).arg(typeString);
}
