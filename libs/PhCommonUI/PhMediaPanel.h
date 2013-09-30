#ifndef PHMEDIAPANEL_H
#define PHMEDIAPANEL_H

#include <QWidget>

#include "PhTools/PhClock.h"

namespace Ui {
	class PhMediaPanel;
}

/**
 * @brief Widget for media control
 *
 * This widget contains the main button, slider and combobox
 * to control a media stream such as video, audio or a graphic strip band.
 *
 * If connected to a clock, it can update it when the user interact with it.
 * Otherwise, only signal are triggered and the element display (timecode text,
 * scrollbar position, button state) when calling the corresponding slots.
 */
class PhMediaPanel : public QWidget
{
	Q_OBJECT

public:
	/**
	* @brief PhMediaPanel constructor
	* @param parent The object owner.
	*/
	explicit PhMediaPanel(QWidget *parent = 0);

	~PhMediaPanel();
	void setTCType(PhTimeCodeType tcType);
	PhTimeCodeType timeCodeType() const;
	void setFirstFrame(PhFrame firstFrame);
	PhFrame getFirstFrame() const;
	void setMediaLength(PhFrame mediaLength);
	PhFrame getMediaLength();

	void setClock(PhClock * clock);

signals:

	void playPause();
	void fastForward();
	void rewind();
	void back();
	void nextFrame();
	void previousFrame();
	void goToFrame(PhFrame frame, PhTimeCodeType tcType);
	void timeCodeTypeChanged(PhTimeCodeType tcType);
public slots:

	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onRateChanged(PhRate rate);

private slots:
	void onPlayPause();
	void onFastForward();
	void onRewind();
	void onBack();
	void onNextFrame();
	void onPreviousFrame();
	void onSliderChanged(int position);
	void onTCTypeComboChanged();


private:

	Ui::PhMediaPanel *ui;
	PhClock *_clock;
	PhFrame _firstFrame;
	PhFrame _mediaLength;//number of frames of the file

};

#endif // PHMEDIAPANEL_H
