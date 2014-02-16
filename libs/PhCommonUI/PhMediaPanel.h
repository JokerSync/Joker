/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

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
	/**
	 * @brief set TC Type
	 * @param tcType The type coming from PhTimeCodeType
	 */
	void setTCType(PhTimeCodeType tcType);
	/**
	 * @brief set the first Frame
	 * @param firstFrame Desired PhFrame
	 */
	void setFirstFrame(PhFrame firstFrame);
	/**
	 * @brief set the media length
	 * Set the media length and adjust the slider
	 *
	 * @param length the desired length
	 */
	void setMediaLength(PhFrame length);
	/**
	 * @brief set Slider Enable
	 * @param isEnabled : true if the slider must be enabled, false otherwise.
	 */
	void setSliderEnable(bool isEnabled);

	/**
	 * @brief set the clock
	 * @param clock the clock which will master the PhMediaPanel
	 */
	void setClock(PhClock * clock);


	/**
	 * @brief get the first PhFrame
	 * @return the corresponding PhFrame
	 */
	PhFrame getFirstFrame() const;

	/**
	 * @brief get the media Length
	 * @return _mediaLength the media length
	 */
	PhFrame getMediaLength();

	/**
	 * @brief get the timecode Type
	 * @return a PhTimeCodeType from the enum
	 */
	PhTimeCodeType timeCodeType() const;

signals:

	/**
	 * @brief Send signal when the play/pause button is pressed.
	 */
	void playPause();
	/**
	 * @brief Send signal when the fastForward button is pressed.
	 */
	void fastForward();
	/**
	 * @brief Send signal when the rewind button is pressed.
	 */
	void rewind();
	/**
	 * @brief Send signal when the back button is pressed.
	 */
	void back();
	/**
	 * @brief Send signal when the nextFrame button is pressed.
	 */
	void nextFrame();
	/**
	 * @brief Send signal when the previousFrame button is pressed.
	 */
	void previousFrame();
	/**
	 * @brief Go to the desired frame
	 * @param frame the desired frame
	 * @param tcType the desired PhTimeCodeType
	 */
	void goToFrame(PhFrame frame, PhTimeCodeType tcType);
	/**
	 * @brief Send a signal when the timecode type change
	 * @param tcType the correponding PhTimeCodeType
	 */
	void timeCodeTypeChanged(PhTimeCodeType tcType);
public slots:

	/**
	 * @brief Handle a modicifation of the frame
	 * @param frame the new frame
	 * @param tcType the corresponding PhTimeCodeType
	 */
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	/**
	 * @brief Handle a modification of the playing rate
	 * @param rate the new rate
	 */
	void onRateChanged(PhRate rate);
	/**
	 * @brief handle a modification of the timecode type
	 * @param tcType the new PhTimeCodeType
	 */
	void onTimeCodeTypeChanged(PhTimeCodeType tcType);

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
	PhFrame _mediaLength; //number of frames of the media
};

#endif // PHMEDIAPANEL_H
