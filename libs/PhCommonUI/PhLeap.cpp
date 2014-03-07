/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#if LEAP

#include <QDebug>
#include "PhLeap.h"

using namespace Leap;

PhLeap::PhLeap(QObject *parent) : QObject(parent) {
}

void PhLeap::onFrame(const Controller& controller)
{

	if(controller.frame().fingers().count() == 1) {
		emit setPosition((controller.frame(1).fingers().leftmost().tipPosition().x - controller.frame().fingers().leftmost().tipPosition().x) * 4);
	}
	if(controller.frame().fingers().count() == 2) {
		emit setRate(controller.frame().fingers().leftmost().tipPosition().x / 5);
	}
}

#endif
