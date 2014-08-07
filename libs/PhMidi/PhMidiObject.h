/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHMIDIOBJECT_H
#define PHMIDIOBJECT_H

#include <QObject>
#include <RtMidi.h>

#include "PhTools/PhTimeCode.h"

/**
 * @brief Common PhMidi object behaviour
 */
class PhMidiObject : public QObject
{
	Q_OBJECT
public:
	/**
	 * @brief PhMidiObject constructor
	 */
	PhMidiObject();

	/**
	 * @brief Open a specified midi port.
	 * @param portName The midi port name
	 * @return True if success, false otherwise.
	 */
	virtual bool open(QString portName) = 0;

protected:
	/**
	 * @brief Compute the time code type from a MTC data byte.
	 * @param data The MTC data byte
	 * @return A timecode type value
	 */
	static PhTimeCodeType computeTimeCodeType(unsigned char data);

	/**
	 * @brief Compute the HH data byte from a time code type
	 * @param hh The hour digit
	 * @param tcType The timecode type
	 * @return A MTC data byte containing the timecode type and the hour digit.
	 */
	static unsigned char computeHH(unsigned char hh, PhTimeCodeType tcType);
};

#endif // PHMIDIOBJECT_H
