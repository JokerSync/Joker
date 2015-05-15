/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhGeneric.h"
#include "PhCommonUI/PhUI.h"

#include "PhTools/PhDebug.h"
#include "PhLockableSpinBox.h"

PhLockableSpinBox::PhLockableSpinBox(QWidget *parent) :
	QSpinBox(parent), _pressCounter(0)
{
	setEnabled(false);
	this->installEventFilter(this);
}

bool PhLockableSpinBox::eventFilter(QObject *sender, QEvent *event)
{
	Q_UNUSED(sender);
	switch(event->type()) {
	case QEvent::MouseButtonPress:
		if(_pressTimer.elapsed() > 250)
			_pressCounter = 0;
		PHDEBUG << _pressCounter++;
		_pressTimer.restart();
		if(_pressCounter > 7) {
			PHDEBUG << "switch";
			this->setEnabled(true);
			_pressCounter = 0;
		}
		if(QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) == true)
			this->setEnabled(true);
		break;
	default:
		break;
	}

	return false;
}
