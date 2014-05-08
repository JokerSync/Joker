/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef GRAPHICSTRIPTEST_H
#define GRAPHICSTRIPTEST_H

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphicStrip/PhGraphicStrip.h"

#include "GraphicStripTestSettings.h"

class GraphicStripTest : public PhGraphicView
{
	Q_OBJECT
public:
	explicit GraphicStripTest(QWidget *parent = 0);

protected:
	bool init();
	void paint();

private slots:
	void testStripDocObject();

private:
	PhGraphicStrip _strip;
	GraphicStripTestSettings _settings;

};

#endif // GRAPHICSTRIPTEST_H
