/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHMIDIOBJECT_H
#define PHMIDIOBJECT_H

#include <QObject>
#include <RtMidi.h>

class PhMidiObject : public QObject
{
	Q_OBJECT
public:
	PhMidiObject();

	virtual bool open(QString portName) = 0;
	virtual void close() = 0;
};

#endif // PHMIDIOBJECT_H
