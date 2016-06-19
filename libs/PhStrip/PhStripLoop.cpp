/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */


#include "PhStripLoop.h"

PhStripLoop::PhStripLoop(PhTime timeIn, QString label) : PhStripObject(timeIn)
{
	_label = label;
}

void PhStripLoop::setLabel(const QString &label)
{
	_label = label;
}
