#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QMainWindow>
#include <QString>

#include "PhTools/PhClock.h"
#include "PhVideo/PhVideoView.h"
#include "PhCommonUI/PhMediaPanel.h"

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

public slots:


private slots:

	void onOpenFile();

	void updateFrame(qint64 position);

	/**
	 * @brief changeValuePlayButton
	 * Change the state of _playButtonState when cliking on _playButton
	 */
	void pushPlayButton();

	/**
	 * @brief changeStateFastForwardButton
	 * Change the state of _fastForwardButtonState when clicking on _fastForwardButton
	 */
	void pushForwardButton();

	/**
	 * @brief changeStateFastRewindButton
	 * Change the state of _fastForwardButtonState when clicking on _fastForwardButton
	 */
	void pushRewindButton();

	void pushBackButton();

	void pushNextFrameButton();

	void pushPreviousFrameButton();

	void useSliderCursor(int position);

	void selectRate(int index);

	void backToBeginning();

private:

	Ui::MainView *ui;
	PhClock *_clock;
};

#endif
