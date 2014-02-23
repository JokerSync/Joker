#ifndef PHWINDOWSETTINGS_H
#define PHWINDOWSETTINGS_H

#include <QByteArray>

/**
 * @brief The settings for PhDocumentWindow
 */
class PhWindowSettings
{
public:
//	virtual bool fullScreen() = 0;
//	virtual void setFullScreen(bool fullScreen) = 0;

	/**
	 * @brief Get the window geometry
	 * @return A byte array
	 */
	virtual QByteArray windowGeometry() = 0;
	/**
	 * @brief Get the window geometry
	 * @param geometry A byte array
	 */
	virtual void setWindowGeometry(QByteArray geometry) = 0;
};

#endif // PHWINDOWSETTINGS_H
