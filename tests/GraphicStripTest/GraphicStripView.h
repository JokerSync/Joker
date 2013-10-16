#ifndef GRAPHICSTRIPVIEW_H
#define GRAPHICSTRIPVIEW_H

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphicStrip/PhGraphicStrip.h"

class GraphicStripView : public PhGraphicView
{
	Q_OBJECT
public:
	explicit GraphicStripView(QWidget *parent = 0);

	PhGraphicStrip * strip() { return &_strip;}
protected:
	bool init();
	void paint();

private:
	PhGraphicStrip _strip;
};

#endif // GRAPHICSTRIPVIEW_H
