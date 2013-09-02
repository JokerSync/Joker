#ifndef PHMEDIAPANEL_H
#define PHMEDIAPANEL_H

#include <QWidget>
#include <QDebug>

#include "PhTools/PhTime.h"
#include "PhTools/PhTimeCode.h"

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
	void setFirstFrame(PhFrame firstFrame);
	PhFrame getFirstFrame() const;
	void setMediaLength(PhFrame mediaLength);
	PhFrame getMediaLength();

signals:

	void playButtonSignal();
	void forwardButtonSignal();
	void rewindButtonSignal();
	void backButtonSignal();
	void nextFrameButtonSignal();
	void previousFrameButtonSignal();
	void useSliderCursorSignal(); // TODO add parameter
	void tcTypeChanged(PhTimeCodeType tcType);

public slots:
	void onFrameChanged();
	void onRateChanged(PhRate rate);

private slots:
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


private:

	Ui::PhMediaPanel *ui;
	PhTimeCodeType _tcType;
	PhFrame _firstFrame;
	PhFrame _mediaLength;//number of frames of the file

};

#endif // PHMEDIAPANEL_H
