#ifndef VIDEOTESTSETTINGS_H
#define VIDEOTESTSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhVideo/PhVideoSettings.h"

class VideoTestSettings : PhGenericSettings, public PhVideoSettings
{
public:
	// PhGraphicSettings :
	int screenDelay();
	int screenRefreshInterval();

	// PhVideoSettings :
	bool videoDeinterlace();

	// Other settings :
	void setLastFile(QString lastFile);
	QString lastFile();

};

#endif // VIDEOTESTSETTINGS_H
