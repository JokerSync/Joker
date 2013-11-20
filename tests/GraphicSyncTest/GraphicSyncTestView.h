#ifndef GRAPHICSYNCTESTVIEW_H
#define GRAPHICSYNCTESTVIEW_H

#include "PhGraphic/PhGraphicView.h"
#include "PhGraphic/PhGraphicText.h"

class GraphicSyncTestView : public PhGraphicView
{
	Q_OBJECT
public:
	explicit GraphicSyncTestView(QWidget *parent = 0);

	void setText(QString content);
protected:
	bool init();
	void paint();

private:
	PhFont font;
	PhGraphicText text;
};

#endif // GRAPHICSYNCTESTVIEW_H
