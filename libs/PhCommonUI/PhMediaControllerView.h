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
	explicit PhMediaControllerView(PhClock *clock, QWidget *parent = 0);

	~PhMediaControllerView();

signals:

	void playButtonSignal();
	void forwardButtonSignal();
	void rewindButtonSignal();
	void backButtonSignal();

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

	
private:
	Ui::PhMediaControllerView *ui;
	PhClock *_clock;
};

#endif // PHMEDIACONTROLLERVIEW_H
