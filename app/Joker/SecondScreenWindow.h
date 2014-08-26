#ifndef SECONDSCREENWINDOW_H
#define SECONDSCREENWINDOW_H

#include "PhGraphic/PhGraphicView.h"
#include "PhVideo/PhVideoEngine.h"

#include "JokerSettings.h"

/**
 * @brief The second window widget
 */
class SecondScreenWindow : public PhGraphicView
{
	Q_OBJECT
public:
	/**
	 * @brief The SecondScreenWindow constructor
	 * @param videoEngine The video engine
	 * @param settings The application settings
	 */
	explicit SecondScreenWindow(PhVideoEngine *videoEngine, JokerSettings *settings);

signals:
	/**
	 * @brief Triggered when the second window is closed
	 * @param closedFromUser True if closed from user, false otherwise
	 */
	void closing(bool closedFromUser);

protected:
	/**
	 * @brief Called when the second window is closed
	 * @param event Event information
	 */
	void closeEvent(QCloseEvent *event);

	/**
	 * @brief Called when any event is sent to the second window
	 *
	 * This event filter method check the double click that turn the second window fullscreen.
	 *
	 * @param event Event information
	 * @return True if handled, false otherwise
	 */
	bool eventFilter(QObject *, QEvent *event);

private slots:
	void onPaint(int width, int height);

private:
	PhVideoEngine *_videoEngine;
	JokerSettings *_jokerSettings;
};

#endif // SECONDSCREENWINDOW_H
