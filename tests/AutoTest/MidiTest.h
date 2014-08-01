/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef MIDITEST_H
#define MIDITEST_H

#include <QObject>

class MidiTest : public QObject
{
	Q_OBJECT
private slots:
	void testConnection();
};

#endif // MIDITEST_H
