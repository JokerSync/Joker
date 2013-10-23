#ifndef PHGRAPHICSTRIPVIEW_H
#define PHGRAPHICSTRIPVIEW_H

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphicStrip/PhGraphicStrip.h"

class PhGraphicStripView : public PhGraphicView
{
	Q_OBJECT
public:
	explicit PhGraphicStripView(QWidget *parent = 0);

	PhGraphicStrip * strip() { return &_strip;}
protected:
	bool init();
	void paint();

private:
	PhGraphicStrip _strip;
};

#endif // PHGRAPHICSTRIPVIEW_H
