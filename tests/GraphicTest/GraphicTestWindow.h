#ifndef GRAPHICTESTWINDOW_H
#define GRAPHICTESTWINDOW_H

#include "PhCommonUI/PhWindow.h"

#include "PhGraphic/PhGraphicImage.h"
#include "PhGraphic/PhFont.h"
#include "PhGraphic/PhGraphicSolidRect.h"
#include "PhGraphic/PhGraphicTexturedRect.h"

#include "GraphicTestSettings.h"

namespace Ui {
class GraphicTestWindow;
}

class GraphicTestWindow : public PhWindow
{
	Q_OBJECT

public:
	explicit GraphicTestWindow(GraphicTestSettings *settings);
	~GraphicTestWindow();

private slots:
	void on_actionChange_font_triggered();

	void on_actionSave_triggered();

	void onPaint(int width, int height);

private:
	Ui::GraphicTestWindow *ui;
	GraphicTestSettings *_settings;
	PhGraphicImage _image;
	PhFont _font1, _font2;
	PhGraphicSolidRect _rect;
	PhGraphicTexturedRect _yuvRect, _rgbRect;
	int _x;
};

#endif // GRAPHICTESTWINDOW_H
