#ifndef FORMTESTSETTINGS_H
#define FORMTESTSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"
#include "PhCommonUI/PhFeedbackSettings.h"

class FormTestSettings : protected PhGenericSettings, public PhWindowSettings, public PhFeedbackSettings
{
	Q_OBJECT

public:
	// PhWindowSettings
	PH_SETTING_BOOL(setFullScreen, fullScreen)
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)

	// PhFeedbackSettings
	QString settingsFileName() {
		return _settings.fileName();
	}
	PH_SETTING_STRINGLIST(setEmailList, emailList)
};

#endif // FORMTESTSETTINGS_H
