#ifndef PHWINDOWSETTINGS_H
#define PHWINDOWSETTINGS_H

#include <QByteArray>

/**
 * @brief The settings for PhDocumentWindow
 */
class PhWindowSettings
{
public:
	/**
	 * @brief The window full screen status
	 * @return True if full screen, false otherwise
	 */
	virtual bool fullScreen() = 0;
	/**
	 * @brief Store the fullscreen status
	 * @param fullScreen True if full screen, false otherwise
	 */
	virtual void setFullScreen(bool fullScreen) = 0;

	/**
	 * @brief Get the last window geometry
	 * @return A byte array
	 */
	virtual QByteArray windowGeometry() = 0;
	/**
	 * @brief Store the window geometry
	 * @param geometry A byte array
	 */
	virtual void setWindowGeometry(QByteArray geometry) = 0;
};

#endif // PHWINDOWSETTINGS_H
