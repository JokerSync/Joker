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
	 * @brief PhWindowSettings constructor
	 */
	PhWindowSettings() : _fullScreen(false) {}

	/**
	 * @brief The window full screen status
	 * @return True if full screen, false otherwise
	 */
	virtual bool fullScreen() { return _fullScreen; }
	/**
	 * @brief Store the fullscreen status
	 * @param fullScreen True if full screen, false otherwise
	 */
	virtual void setFullScreen(bool fullScreen) { _fullScreen = fullScreen; }

	/**
	 * @brief Get the last window geometry
	 * @return A byte array
	 */
	virtual QByteArray windowGeometry() { return QByteArray(); }
	/**
	 * @brief Store the window geometry
	 * @param geometry A byte array
	 */
	virtual void setWindowGeometry(QByteArray geometry) {};

private:
	bool _fullScreen;
};

#endif // PHWINDOWSETTINGS_H
