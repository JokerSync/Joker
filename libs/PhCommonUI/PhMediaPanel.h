/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHMEDIAPANEL_H
#define PHMEDIAPANEL_H

#include <QWidget>

#include "PhSync/PhClock.h"

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
	 * @brief The PhMediaPanel constructor
	 * @param parent The object owner.
	 */
	PhMediaPanel(QWidget *parent = 0);

	~PhMediaPanel();
	/**
	 * @brief set TC Type
	 * @param tcType The type coming from PhTimeCodeType
	 */
	void setTCType(PhTimeCodeType tcType);
	/**
	 * @brief set the first Frame
	 * @param firstFrame Desired PhFrame
	 * @todo remove
	 */
	void setFirstFrame(PhFrame frameIn);

	/**
	 * @brief Set the starting time of the panel
	 * @param timeIn A time value
	 */
	void setTimeIn(PhTime timeIn);

	/**
	 * @brief Set the media length in frame and adjust the slider
	 * @param length A frame value
	 * @todo remove
	 */
	void setMediaLength(PhFrame length);

	/**
	 * @brief Set the media length and adjust the slider
	 * @param length A time value
	 */
	void setLength(PhTime length);

	/**
	 * @brief Enable the panel slider
	 * @param isEnabled True if the slider must be enabled, false otherwise.
	 */
	void setSliderEnable(bool isEnabled);

	/**
	 * @brief Set the panel clock
	 * @param clock A clock instance
	 *
	 * The panel will display the value of the clock.
	 */
	void setClock(PhClock * clock);


	/**
	 * @brief Get the first frame
	 * @return A frame value
	 * @todo remove
	 */
	PhFrame getFirstFrame() const;

	/**
	 * @brief Get the starting time
	 * @return A time value
	 */
	PhTime timeIn() const;

	/**
	 * @brief Get the media length in frame
	 * @return A frame value
	 * @todo remove
	 */
	PhFrame getMediaLength();

	/**
	 * @brief Get the media panel length
	 * @return
	 */
	PhTime length();

	/**
	 * @brief Get the panel timecode type
	 * @return A timecode type value.
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
	PhTime _timeIn;
	PhTime _length; //number of frames of the media
};

#endif // PHMEDIAPANEL_H
