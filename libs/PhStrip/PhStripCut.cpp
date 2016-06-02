/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStripCut.h"

PhStripCut::PhStripCut(PhTime time, PhStripCut::PhCutType type) : PhStripObject(time)
{
	_type = type;
}

QString PhStripCut::description(PhTimeCodeType tcType)
{
	QString typeString = "";
	switch (this->_type) {
	case Simple:
		typeString = "Simple";
		break;
	case FadeIn:
		typeString = "Simple";
		break;
	case FadeOut:
		typeString = "FadeOut";
		break;
	case CrossFade:
		typeString = "CrossFade";
		break;
	}
	return QString("%1 - %2").arg(this->tcIn(tcType)).arg(typeString);
}
