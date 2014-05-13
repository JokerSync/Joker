#ifndef GRAPHICTESTWINDOW_H
#define GRAPHICTESTWINDOW_H

#include "PhCommonUI/PhWindow.h"

#include "GraphicTestSettings.h"
#include "GraphicTestView.h"

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

	void onPaint();

private:
	Ui::GraphicTestWindow *ui;
	GraphicTestSettings *_settings;
	PhGraphicImage _image;
	PhFont _font1, _font2;
	PhGraphicSolidRect _rect;
	PhGraphicTexturedRect _yuvRect;
	int _x;
};

#endif // GRAPHICTESTWINDOW_H
