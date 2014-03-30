#ifndef TEXTEDITTESTSETTINGS_H
#define TEXTEDITTESTSETTINGS_H

#include <QDir>

#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"
#include "PhCommonUI/PhFeedbackSettings.h"

class TextEditTestSettings : protected PhGenericSettings, public PhDocumentWindowSettings, public PhFeedbackSettings
{
public:
	// PhWindowSettings
	PH_SETTING_BOOL(setFullScreen, fullScreen)
	PH_SETTING_BOOL(setExitedNormaly, exitedNormaly)
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)

	// PhDocumentWindowSettings
	PH_SETTING_STRING(setCurrentDocument, currentDocument)
	PH_SETTING_STRING2(setLastDocumentFolder, lastDocumentFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(setRecentDocumentList, recentDocumentList)
	PH_SETTING_INT2(setMaxRecentDocument, maxRecentDocument, 5)
	PH_SETTING_BOOL2(setAutoReload, autoReload, true)

	// PhFeedbackSettings
	QString settingsFileName() {
		return _settings.fileName();
	}
	PH_SETTING_STRINGLIST(setEmailList, emailList)
};

#endif // TEXTEDITTESTSETTINGS_H
