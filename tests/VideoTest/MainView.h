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

#include "PhVideoSynchronizer.h"

using namespace std;

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QSlider;
class QLabel;
QT_END_NAMESPACE

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

    void onTimeOut();

	void on_actionOpen_triggered();

private:

	Ui::MainView *ui;
    PhClock _internalClock;
	QTimer timer;
    PhVideoSynchronizer _synchronizer;
	PhVideoEngine _videoEngine;
};

#endif
