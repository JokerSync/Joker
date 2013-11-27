#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QSettings>
#include <QPropertyAnimation>

#include "VideoStripView.h"
#include "PhSync/PhSonySlaveController.h"
#include "PhCommonUI/PhMediaPanelDialog.h"

#include "SonyVideoStripSynchronizer.h"
#include "PropertyDialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QSettings *settings);
	~MainWindow();

	void openFile(QString fileName);

	bool openVideoFile(QString videoFileName);

	bool openStripFile(QString stripFileName);

protected:
	bool eventFilter(QObject *sender, QEvent *event);

	enum MediaPanelState {
		MediaPanelVisible,
		MediaPanelHidding,
		MediaPanelHidden
	};


private slots:
		// Qt Designer slots
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

	void on_actionOpen_Video_triggered();

	void on_actionChange_timestamp_triggered();

	void on_actionAbout_triggered();

	void on_actionPreferences_triggered();

	void on_actionClear_list_triggered();


	// Custom slots

	void fadeInMediaPanel();

	void fadeOutMediaPanel();

	void hideMediaPanel();

	void on_actionProperties_triggered();

	void on_actionTest_mode_triggered();

	void on_actionTimecode_triggered();

	void on_actionNext_element_triggered();

	void on_actionPrevious_element_triggered();

	void openRecent();

	void on_actionImport_triggered();

	void on_actionSave_triggered();

private:
	Ui::MainWindow *ui;
	PhGraphicStrip * _strip;
	PhVideoEngine * _videoEngine;
	PhStripDoc *_doc;
	QSettings *_settings;
	PhSonySlaveController _sonySlave;
	VideoStripSynchronizer _synchronizer;

	PhMediaPanelDialog _mediaPanel;
	QTimer _mediaPanelTimer;
	MediaPanelState _mediaPanelState;
	QPropertyAnimation _mediaPanelAnimation;

	PropertyDialog _propertyDialog;

	QVector<QAction *> _recentFileButtons;

	void updateOpenRecent();
	void setupOpenRecentMenu();
	void closeEvent(QCloseEvent *event);
	bool saveStrip();
};

#endif // MAINWINDOW_H
