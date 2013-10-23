#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>

#include "PhTools/PhClock.h"
#include "PhTools/PhDebug.h"

#include "PhVideo/PhVideoView.h"
#include "PhVideo/PhVideoEngine.h"

#include "PhCommonUI/PhMediaPanel.h"

namespace Ui {
	class MainView;
}

class MainView : public QMainWindow
{
    Q_OBJECT
public:
    /**
     * @brief MainView constructor
     */
    MainView();
	~MainView();
    /**
     * Open a video file.
     * @param fileName Path of the video file to open.
     * @return True if succeeds, false otherwise.
     */
    bool openFile(QString fileName);

private slots:
	void on_actionPlay_pause_triggered();

	void on_actionNext_frame_triggered();

	void on_actionPrevious_frame_triggered();

	void on_actionSet_timestamp_triggered();

	void on_actionOpen_triggered();

	void on_actionReverse_triggered();

private:

	Ui::MainView *ui;
	PhVideoEngine _videoEngine;
};

#endif
