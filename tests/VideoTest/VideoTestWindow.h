#ifndef VIDEOTESTWINDOW_H
#define VIDEOTESTWINDOW_H

#include <QString>
#include <QTimer>

#include "PhSync/PhClock.h"
#include "PhTools/PhDebug.h"

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
	bool openDocument(const QString &fileName);

	/**
	 * @brief Process the application argument
	 *
	 * Set the timestamp if provided in the argument
	 *
	 * @param argc The arguments count
	 * @param argv The arguments values
	 */
	void processArg(int argc, char *argv[]);

protected:
	bool eventFilter(QObject *sender, QEvent *event);
	void resizeEvent(QResizeEvent *);
	void closeEvent(QCloseEvent *);

	QMenu *recentDocumentMenu();
	QAction *fullScreenAction() {
		return NULL;
	}

	void onApplicationActivate();
	void onApplicationDeactivate();

private slots:
	void videoFileOpened(bool success);

	void on_actionPlay_pause_triggered();

	void on_actionNext_frame_triggered();

	void on_actionPrevious_frame_triggered();

	void on_actionSet_timestamp_triggered();

	void on_actionOpen_triggered();

	void on_actionReverse_triggered();

	void on_actionGo_to_triggered();

	void on_actionDisplay_media_panel_triggered(bool checked);

	void on_actionDeinterlace_video_triggered(bool checked);

	void onTimeChanged(PhTime time);

	void onPaint(int width, int height);
	void on_actionProperties_triggered();

	void on_actionUse_native_video_size_triggered(bool checked);

	void on_actionPicture_in_picture_triggered(bool checked);

private:

	Ui::VideoTestWindow *ui;
	VideoTestSettings *_settings;
	PhVideoEngine _videoEngine;
	PhFloatingMediaPanel _mediaPanelDialog;
	int _maxVideoRate;
};

#endif
