#ifndef VIDEOTESTWINDOW_H
#define VIDEOTESTWINDOW_H

#include <QString>
#include <QTimer>

#include "PhTools/PhClock.h"
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
	bool openDocument(QString fileName);

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

	void on_actionGo_to_triggered();

	void on_actionDisplay_media_panel_triggered(bool checked);

	void on_actionDeinterlace_video_triggered(bool checked);

	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);

	void onPaint(int width, int height);
private:

	Ui::VideoTestWindow *ui;
	VideoTestSettings *_settings;
	PhVideoEngine _videoEngine;
	PhFloatingMediaPanel _mediaPanelDialog;
	int _maxVideoRate;
};

#endif
