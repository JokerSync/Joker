#ifndef PHMEDIACONTROLLERVIEW_H
#define PHMEDIACONTROLLERVIEW_H

#include <QWidget>
#include <PhTools/PhClock.h>


namespace Ui {
class PhMediaControllerView;
}

class PhMediaControllerView : public QWidget
{
	Q_OBJECT
	
public:
	explicit PhMediaControllerView(PhClock *clock, PhTimeCodeType timecodeType, PhFrame lenghtFile, PhFrame firstFrame, QWidget *parent = 0);
	int get_framePerSecond() const;

	~PhMediaControllerView();

signals:

	void playButtonSignal();
	void forwardButtonSignal();
	void rewindButtonSignal();
	void backButtonSignal();
	void pushNextFrameButtonSignal();
	void pushPreviousFrameButtonSignal();
	void useSliderCursorSignal();

public slots:
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

	/**
	 * @brief pushBacButton
	 */
	void pushBackButton();

	void pushNextFrameButton();

	void pushPreviousFrameButton();

	void useSliderCursor(int position);

	/**
	 * @brief selectRate
	 */
	void selectRate();

	/**
	 * @brief increaseValueTimecode
	 * Update the timecode every 40*rateValue ms when play button is enabled
	 */
	void updateFrame();


	/**
	 * @brief updateRateDisplay
	 */
	void updateRateLabel();

	/**
	 * @brief updateFrameLabel
	 */
	void updateFrameLabel();
	/**
	 * @brief updateSliderPosition
	 */
	void updateSliderPosition();

	
private:

	PhFrame _lengthFile;//number of frames of the file
	PhFrame _firstFrame;
	int _fileProgress;
	Ui::PhMediaControllerView *ui;
	PhClock *_clock;
	PhTimeCodeType _timecodeType;
	int _framePerSecond;
};

#endif // PHMEDIACONTROLLERVIEW_H
