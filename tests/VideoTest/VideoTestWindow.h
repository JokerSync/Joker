#ifndef VIDEOTESTWINDOW_H
#define VIDEOTESTWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QSettings>

#include "PhTools/PhClock.h"
#include "PhTools/PhDebug.h"

#include "PhVideo/PhVideoView.h"
#include "PhVideo/PhVideoEngine.h"

#include "PhCommonUI/PhMediaPanelDialog.h"

namespace Ui {
	class VideoTestWindow;
}

class VideoTestWindow : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief MainView constructor
     */
    VideoTestWindow(QSettings *settings);

	~VideoTestWindow();
    /**
     * Open a video file.
     * @param fileName Path of the video file to open.
     * @return True if succeeds, false otherwise.
     */
    bool openFile(QString fileName);

protected:
	void resizeEvent(QResizeEvent *);

private slots:
	void on_actionPlay_pause_triggered();

	void on_actionNext_frame_triggered();

	void on_actionPrevious_frame_triggered();

	void on_actionSet_timestamp_triggered();

	void on_actionOpen_triggered();

	void on_actionReverse_triggered();

private:

	Ui::VideoTestWindow *ui;
	QSettings *_settings;
	PhVideoEngine _videoEngine;
	PhMediaPanelDialog _mediaPanelDialog;
};

#endif
