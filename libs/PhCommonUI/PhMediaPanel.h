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
	 * @brief Get the panel timecode type
	 * @return A timecode type value.
	 */
	PhTimeCodeType timeCodeType() const;

	/**
	 * @brief Set type timecode type used to display the time.
	 * @param tcType A timecode type
	 */
	void setTimeCodeType(PhTimeCodeType tcType);

	/**
	 * @brief Set the clock that the panel will display
	 * @param clock A clock instance.
	 */
	void setClock(PhClock * clock);

	/**
	 * @brief Enable the panel slider
	 * @param enabled True if the slider must be enabled, false otherwise.
	 */
	void setSliderEnable(bool enabled);

	/**
	 * @brief Enable the dropdown allowing selecting speed
	 * @param enabled True if the dropdown must be enabled, false otherwise.
	 */
	void setDropdownEnable(bool enabled);

	/**
	 * @brief Get the starting time
	 * @return A time value
	 */
	PhTime timeIn() const;

	/**
	 * @brief Set the starting time of the panel
	 * @param timeIn A time value
	 */
	void setTimeIn(PhTime timeIn);

	/**
	 * @brief Get the media panel length
	 * @return
	 */
	PhTime length();
	/**
	 * @brief Set the media length and adjust the slider
	 * @param length A time value
	 */
	void setLength(PhTime length);

	/**
	 * @brief Return the playback status of the panel
	 * @return A bool value.
	 */
	bool isPlaying();

signals:

	/**
	 * @brief Send signal when the play button is clicked.
	 */
	void playClicked();
	/**
	 * @brief Send signal when the pause button is clicked.
	 */
	void pauseClicked();
	/**
	 * @brief Send signal when the fastForward button is pressed.
	 */
	void fastForwardClicked();
	/**
	 * @brief Send signal when the rewind button is pressed.
	 */
	void rewindClicked();
	/**
	 * @brief Send signal when the back button is pressed.
	 */
	void backClicked();
	/**
	 * @brief Send signal when the nextFrame button is pressed.
	 */
	void nextFrameClicked();
	/**
	 * @brief Send signal when the previousFrame button is pressed.
	 */
	void previousFrameClicked();
	/**
	 * @brief The slider has moved to a specific time
	 * @param time A time value.
	 */
	void sliderMoved(PhTime time);
	/**
	 * @brief Send a signal when the timecode type change
	 * @param tcType The new PhTimeCodeType
	 */
	void timeCodeTypeChanged(PhTimeCodeType tcType);
public slots:
	/**
	 * @brief Handle a modification of the time
	 * @param time The new time
	 */
	void onTimeChanged(PhTime time);
	/**
	 * @brief Handle a modification of the playing rate
	 * @param rate the new rate
	 */
	void onRateChanged(PhRate rate);
	/**
	 * @brief Handle a modification of the timecode type
	 * @param tcType The new timecode type.
	 */
	void onTimeCodeTypeChanged(PhTimeCodeType tcType);

protected:
	/**
	 * @brief Filter the space key press event
	 * @param event The event
	 * @return True if the event has been handled, false otherwise
	 */
	bool eventFilter(QObject *, QEvent *event);

private slots:
	void onPlayPause();
	void onFastForward();
	void onRewind();
	void onBack();
	void onNextFrame();
	void onPreviousFrame();
	void onSliderChanged(int position);
	void updateSlider();
	void onTCTypeComboChanged();

	void updatePlayingState();

private:
	Ui::PhMediaPanel *ui;
	PhClock *_clock;
	PhTime _timeIn;
	PhTime _length;
	bool _playing;
};

#endif // PHMEDIAPANEL_H
