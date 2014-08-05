/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHMIDIOBJECT_H
#define PHMIDIOBJECT_H

#include <QObject>
#include <RtMidi.h>

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
};

#endif // PHMIDIOBJECT_H
