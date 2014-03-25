#ifndef VIDEOTESTWINDOW_H
#define VIDEOTESTWINDOW_H

#include <QString>
#include <QTimer>

#include "PhTools/PhClock.h"
#include "PhTools/PhDebug.h"

#include "PhVideo/PhVideoView.h"
#include "PhVideo/PhVideoEngine.h"

#include "PhCommonUI/PhFloatingMediaPanel.h"
#include "PhCommonUI/PhDocumentWindow.h"

#include "VideoTestSettings.h"

namespace Ui {
class VideoTestWindow;
}

class VideoTestWindow : public PhDocumentWindow
{
	Q_OBJECT
public:
	/**
	 * @brief MainView constructor
	 */
	VideoTestWindow(VideoTestSettings *settings);

	~VideoTestWindow();
	/**
	 * Open a video file.
	 * @param fileName Path of the video file to open.
	 * @return True if succeeds, false otherwise.
	 */
	bool openDocument(QString fileName);

protected:
	void resizeEvent(QResizeEvent *);
	void closeEvent(QCloseEvent *);

	QMenu *recentDocumentMenu();
	QAction *fullScreenAction() {
		return NULL;
	}

	void onApplicationActivate();
	void onApplicationDeactivate();

private slots:
	void on_actionPlay_pause_triggered();

	void on_actionNext_frame_triggered();

	void on_actionPrevious_frame_triggered();

	void on_actionSet_timestamp_triggered();

	void on_actionOpen_triggered();

	void on_actionReverse_triggered();

private:

	Ui::VideoTestWindow *ui;
	VideoTestSettings *_settings;
	PhVideoEngine _videoEngine;
	PhFloatingMediaPanel _mediaPanelDialog;
};

#endif
