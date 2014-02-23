#ifndef PHWINDOW_H
#define PHWINDOW_H

#include <QKeyEvent>
#include <QMainWindow>

#include "PhWindowSettings.h"

/**
 * @brief Common window behaviour for application
 *
 * This class provide common user interface mechanism:
 * - Save window geometry
 */
class PhWindow : public QMainWindow
{
	Q_OBJECT
public:
	/**
	 * @brief PhWindow constructor
	 * @param settings The window settings
	 */
	explicit PhWindow(PhWindowSettings *settings);

protected:
	/**
	 * @brief Custom event filter
	 *
	 * The event filter catch the following event:
	 * - WindowStateChange: to update the fullscreen setting
	 *
	 * @param sender The event sender
	 * @param event The event
	 * @return True if handled, false otherwise
	 */
	virtual bool eventFilter(QObject *sender, QEvent *event);
	/**
	 * @brief moveEvent Store the window geometry in the settings
	 */
	void moveEvent(QMoveEvent *);
	/**
	 * @brief resizeEvent Store the window geometry in the settings
	 */
	void resizeEvent(QResizeEvent *);

	virtual QAction *fullScreenAction() { return NULL;}

protected slots:
	void toggleFullScreen();

private:
	PhWindowSettings *_settings;
};

#endif // PHWINDOW_H
