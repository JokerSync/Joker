#ifndef WINDOWSPECSETTINGS_H
#define WINDOWSPECSETTINGS_H

#include <QDir>

#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class WindowSpecSettings : public PhGenericSettings, public PhDocumentWindowSettings
{
public:
	WindowSpecSettings() : PhGenericSettings(true) {
	}

	// PhWindowSettings
	//PH_SETTING_BOOL(setFullScreen, fullScreen) // already implemented in PhWindowSettings
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)

	// PhDocumentWindowSettings
	PH_SETTING_STRING(setCurrentDocument, currentDocument)
	PH_SETTING_STRING2(setLastDocumentFolder, lastDocumentFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(setRecentDocumentList, recentDocumentList)
	PH_SETTING_INT2(setMaxRecentDocument, maxRecentDocument, 5)
	PH_SETTING_BOOL2(setAutoReload, autoReload, true)
};

#endif // WINDOWSPECSETTINGS_H
