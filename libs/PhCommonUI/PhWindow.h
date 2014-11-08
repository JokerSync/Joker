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

public slots:
	/**
	 * @brief Toggle between fullscreen and normal
	 */
	void toggleFullScreen();

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

	/**
	 * @brief The fullScreen action
	 *
	 * In order to update the fullScreen action check
	 * according to the fullscreen test (trigger by the
	 * toogleFullscreen() slot or by clicking the fullscreen
	 * top right icon (under MacOS), the child window must
	 * implement this method to give the action to PhWindow.
	 * The action must be connected by the child
	 * to the toogleFullscreen() slot.
	 *
	 * @return An action reference
	 */
	virtual QAction *fullScreenAction() {
		return NULL;
	}

	/**
	 * @brief Called when the application is activated
	 */
	virtual void onApplicationActivate();

	/**
	 * @brief Called when the application is deactivated
	 */
	virtual void onApplicationDeactivate();

private slots:
	void onApplicationStateChange(Qt::ApplicationState state);

private:
	PhWindowSettings *_settings;
};

#endif // PHWINDOW_H
