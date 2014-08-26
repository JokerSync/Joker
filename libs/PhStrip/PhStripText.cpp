/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhStripText.h"
#include "PhTools/PhDebug.h"

PhStripText::PhStripText(QString content, PhTime duration, PhStripDetect::PhDetectType typeOut) :
	_content(content),
	_duration(duration),
	_typeOut(typeOut)
{
}

QString PhStripText::content() const
{
	return _content;
}

void PhStripText::setContent(QString content)
{
	if (content != _content) {
		_content = content;
		emit contentChanged();
	}
}

PhStripDetect::PhDetectType PhStripText::typeOut() const
{
	return _typeOut;
}

void PhStripText::setTypeOut(const PhStripDetect::PhDetectType &typeOut)
{
	_typeOut = typeOut;
}

PhTime PhStripText::duration() const
{
	return _duration;
}

void PhStripText::setDuration(const PhTime &duration)
{
	_duration = duration;
}
