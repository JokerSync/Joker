#ifndef GRAPHICSYNCTESTVIEW_H
#define GRAPHICSYNCTESTVIEW_H

#include "PhGraphic/PhGraphicView.h"

class GraphicSyncTestView : public PhGraphicView
{
	Q_OBJECT
public:
	explicit GraphicSyncTestView(QWidget *parent = 0);

protected:
	bool init();
	void paint();
};

#endif // GRAPHICSYNCTESTVIEW_H
