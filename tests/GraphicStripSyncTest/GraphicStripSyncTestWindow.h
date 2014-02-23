#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "PhGraphicStrip/PhGraphicStrip.h"
#include "PhSync/PhSonySlaveController.h"
#include "PhCommonUI/PhDocumentWindow.h"

#include "GraphicStripSyncTestSettings.h"
#include "SonyStripSynchronizer.h"

namespace Ui {
class GraphicStripSyncTestWindow;
}

class GraphicStripSyncTestWindow : public PhDocumentWindow
{
	Q_OBJECT

public:
	explicit GraphicStripSyncTestWindow(GraphicStripSyncTestSettings *settings, QWidget *parent = 0);
	~GraphicStripSyncTestWindow();

	bool openDocument(QString fileName);

protected:
	QMenu *recentDocumentMenu();

private slots:
	void onOpenFile();
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onRateChanged(PhRate rate);

	void on_actionPlay_pause_triggered();

	void on_actionPlay_backward_triggered();

	void on_actionStep_forward_triggered();

	void on_actionStep_backward_triggered();

	void on_actionStep_time_forward_triggered();

	void on_actionStep_time_backward_triggered();

	void on_action_3_triggered();

	void on_action_1_triggered();

	void on_action_0_5_triggered();

	void on_action0_triggered();

	void on_action0_5_triggered();

	void on_action1_triggered();

	void on_action3_triggered();

	void on_actionGo_To_triggered();

	void on_actionPreferences_triggered();

private:
	Ui::GraphicStripSyncTestWindow *ui;
	PhGraphicStrip *_strip;
	PhStripDoc *_doc;
	PhClock *_clock;
	GraphicStripSyncTestSettings *_settings;
	PhSonySlaveController _sonySlave;
	SonyStripSynchronizer _clockSynchroniser;
};

#endif // MAINWINDOW_H
