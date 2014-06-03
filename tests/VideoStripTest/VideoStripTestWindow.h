#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMessageBox>
#include <QTimer>

#include "PhGraphicStrip/PhGraphicStrip.h"
#include "PhVideo/PhVideoEngine.h"
#include "PhStrip/PhStripDoc.h"
#include "PhCommonUI/PhDocumentWindow.h"

#include "VideoStripTestSettings.h"
#include "VideoStripSynchronizer.h"

namespace Ui {
class VideoStripTestWindow;
}

class VideoStripTestWindow : public PhDocumentWindow
{
	Q_OBJECT

public:
	explicit VideoStripTestWindow(VideoStripTestSettings * settings);
	~VideoStripTestWindow();

	bool openDocument(QString fileName);

	bool openVideoFile(QString videoFileName);

protected:
	QMenu *recentDocumentMenu();
	QAction *fullScreenAction() {
		return NULL;
	}

private slots:
	void on_actionOpen_triggered();

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

	void on_actionOpen_Video_triggered();

	void on_actionSet_Time_Code_triggered();

	void on_actionFull_screen_triggered();
	void on_actionTest_mode_triggered();

	void onPaint(int width, int height);
private:
	Ui::VideoStripTestWindow *ui;
	VideoStripTestSettings * _settings;
	PhGraphicStrip _strip;
	PhStripDoc *_doc;
	PhVideoEngine _videoEngine;
	VideoStripSynchronizer _synchronizer;
	QTimer _fullScreenTimer;
};

#endif // MAINWINDOW_H
