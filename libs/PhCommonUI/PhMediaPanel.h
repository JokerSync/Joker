#ifndef PHMEDIAPANEL_H
#define PHMEDIAPANEL_H

#include <QWidget>
#include <PhTools/PhClock.h>
#include <QDebug>


namespace Ui {
class PhMediaPanel;
}

class PhMediaPanel : public QWidget
{
	Q_OBJECT
	
public:
	explicit PhMediaPanel(QWidget *parent = 0);
	~PhMediaPanel();
	void setTCType(PhTimeCodeType tcType);
	PhTimeCodeType getTCType() const;
	void setClock(PhClock *clock);
	PhClock* getClock() const;
	void setFirstFrame(PhFrame firstFrame);
	PhFrame getFirstFrame() const;
	void setMediaLength(qint64 mediaLength);

signals:

	void playButtonSignal();
	void forwardButtonSignal();
	void rewindButtonSignal();
	void backButtonSignal();
	void nextFrameButtonSignal();
	void previousFrameButtonSignal();
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

	void pushBackButton();

	void pushNextFrameButton();

	void pushPreviousFrameButton();

	void useSliderCursor(int position);

	void selectRate();

	void onRateChanged(PhRate rate);

	void onFrameChanged();


private:

	Ui::PhMediaPanel *ui;
	PhTimeCodeType _tcType;
	PhClock *_clock;
	PhFrame _firstFrame;
	PhFrame _mediaLength;//number of frames of the file

};

#endif // PHMEDIAPANEL_H
