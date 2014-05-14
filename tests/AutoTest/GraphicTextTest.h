/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef GRAPHICTEXTTEST_H
#define GRAPHICTEXTTEST_H

#include <QObject>

#include "PhGraphic/PhFont.h"

class GraphicTextTest : public QObject
{
	Q_OBJECT

private slots:
	void testFont();
	void testFont_data();
	void computeMaxFontSizeTest();
};

#endif // GRAPHICTEXTTEST_H
