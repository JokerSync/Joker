#ifndef PHWINDOWSETTINGS_H
#define PHWINDOWSETTINGS_H

#include <QByteArray>

class PhWindowSettings
{
public:
//	virtual bool fullScreen() = 0;
//	virtual void setFullScreen(bool fullScreen) = 0;

	virtual QByteArray windowGeometry() = 0;
	virtual void setWindowGeometry(QByteArray geometry) = 0;
};

#endif // PHWINDOWSETTINGS_H
