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

	void on_actionInfos_triggered(bool checked);

	void on_actionImage_1_triggered(bool checked);

	void on_actionImage_2_triggered(bool checked);

	void on_actionRectangles_triggered(bool checked);

	void on_actionArrows_triggered(bool checked);

	void on_actionDisc_triggered(bool checked);

	void on_actionCharacters_triggered(bool checked);

	void on_actionStatic_text_triggered(bool checked);

	void on_actionMoving_text_triggered(bool checked);

	void on_actionZoom_in_triggered();

	void on_actionZoom_out_triggered();

	void on_actionMove_up_triggered();

	void on_actionMove_right_triggered();

	void on_actionMove_down_triggered();

	void on_actionMove_left_triggered();

	void on_actionChange_text_triggered();

	void onFontSelected(const QFont &font);

private:
	Ui::GraphicTestWindow *ui;
	GraphicTestSettings *_settings;
	PhGraphicImage _image1, _image2;
	PhFont _font1, _font2;
	PhGraphicSolidRect _rect;
	PhGraphicTexturedRect _yuvRect, _rgbRect;
	int _x;
	int _zoom, _xOffset, _yOffset;
};

#endif // GRAPHICTESTWINDOW_H
