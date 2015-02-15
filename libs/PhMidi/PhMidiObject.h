/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHMIDIOBJECT_H
#define PHMIDIOBJECT_H

#include <RtMidi.h>

#include "PhTools/PhGeneric.h"

#include "PhSync/PhTimeCode.h"

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

	/**
	 * @brief close Close the midi port if opened
	 */
	virtual void close() = 0;

	/**
	 * @brief Finds whether the MIDI implementation can use virtual ports
	 * @return True if virtual ports are available, false otherwise.
	 */
	static bool canUseVirtualPorts();

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
	 * @return A full MTC hour digit containing the timecode type and the hour digit.
	 */
	static unsigned char computeHH(unsigned char hh, PhTimeCodeType tcType);

	/**
	 * @brief Compute the hour high digit from a time code type
	 * @param hh The hour digits
	 * @param tcType The timecode type
	 * @return A quarter frame MTC data byte containing the timecode type and the hour high digit.
	 */
	static unsigned char computeH(unsigned char hh, PhTimeCodeType tcType);
};

#endif // PHMIDIOBJECT_H
