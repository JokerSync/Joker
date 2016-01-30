/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHWINDOWSETTINGS_H
#define PHWINDOWSETTINGS_H

/**
 * @brief The settings for PhDocumentWindow
 */
class PhWindowSettings
{
public:
	/**
	 * @brief PhWindowSettings constructor
	 */
	PhWindowSettings() : _fullScreen(false) {
	}

	/**
	 * @brief PhWindowSettings destructor
	 */
	virtual ~PhWindowSettings() {
	}

	/**
	 * @brief The window full screen status
	 * @return True if full screen, false otherwise
	 */
	virtual bool fullScreen() {
		return _fullScreen;
	}
	/**
	 * @brief Store the fullscreen status
	 * @param fullScreen True if full screen, false otherwise
	 */
	virtual void setFullScreen(bool fullScreen) {
		_fullScreen = fullScreen;
	}

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

private:
	bool _fullScreen;
};

#endif // PHWINDOWSETTINGS_H
