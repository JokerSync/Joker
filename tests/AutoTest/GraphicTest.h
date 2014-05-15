/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef GRAPHICTEST_H
#define GRAPHICTEST_H

#include <QObject>

class GraphicTest : public QObject
{
	Q_OBJECT
public:
	explicit GraphicTest(QObject *parent = 0);

private slots:
	void initTestCase();
	void paintTest();
	void rectTest();
	void imageTest();
	void rgbPatternTest();
};

#endif // GRAPHICTEST_H
