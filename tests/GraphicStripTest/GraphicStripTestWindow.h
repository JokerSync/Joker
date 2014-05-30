#ifndef GRAPHICSTRIPTESTWINDOW_H
#define GRAPHICSTRIPTESTWINDOW_H

#include <QMessageBox>

#include "PhGraphicStrip/PhGraphicStrip.h"
#include "PhCommonUI/PhDocumentWindow.h"

#include "StripPropertiesDialog.h"
#include "GenerateDialog.h"
#include "GraphicStripTestSettings.h"

namespace Ui {
class GraphicStripTestWindow;
}

class GraphicStripTestWindow : public PhDocumentWindow
{
	Q_OBJECT

public:
	explicit GraphicStripTestWindow(GraphicStripTestSettings *settings);
	~GraphicStripTestWindow();

	bool openDocument(QString fileName);

protected:
	QMenu *recentDocumentMenu();
	QAction *fullScreenAction();

private slots:
	void onOpenFile();
	void onGenerate();
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onRateChanged(PhRate rate);

	void on_actionPlay_pause_triggered();

	void on_actionPlay_backward_triggered();

	void on_actionStep_forward_triggered();

	void on_actionStep_backward_triggered();

	void on_actionStep_time_forward_triggered();

	void on_actionStep_time_backward_triggered();

	void on_actionPrevious_Element_triggered();

	void on_actionNext_Element_triggered();

	void on_action_3_triggered();

	void on_action_1_triggered();

	void on_action_0_5_triggered();

	void on_action0_triggered();

	void on_action0_5_triggered();

	void on_action1_triggered();

	void on_action3_triggered();

	void on_actionGo_to_triggered();

	void on_actionStrip_Properties_triggered();

	void on_actionInvert_colors_triggered(bool checked);

	void on_actionRuler_triggered(bool checked);

	void on_actionChange_ruler_timestamp_triggered();

	void onPaint(int width, int height);

	void on_actionChange_font_triggered();

protected:
	PhClock *_clock;
private:
	Ui::GraphicStripTestWindow *ui;
	GraphicStripTestSettings *_settings;
	PhGraphicStrip _strip;
	PhStripDoc *_doc;
	StripPropertiesDialog *dlg;
};

#endif // GRAPHICSTRIPTESTWINDOW_H
