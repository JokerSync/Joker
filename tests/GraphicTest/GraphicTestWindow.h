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

private:
	Ui::GraphicTestWindow *ui;
	GraphicTestSettings *_settings;
};

#endif // GRAPHICTESTWINDOW_H
