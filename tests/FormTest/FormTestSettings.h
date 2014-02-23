#ifndef FORMTESTSETTINGS_H
#define FORMTESTSETTINGS_H

#include <QDir>
#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class FormTestSettings : protected PhGenericSettings, public PhDocumentWindowSettings
{
public:
	// PhWindowSettings
//	PH_SETTING_BOOL(setFullScreen, fullScreen)
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)

	// PhDocumentWindowSettings
	PH_SETTING_STRING(setCurrentDocument, currentDocument)
	PH_SETTING_STRING2(setLastDocumentFolder, lastDocumentFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(setRecentDocumentList, recentDocumentList)
	PH_SETTING_INT2(setMaxRecentDocument, maxRecentDocument, 5)
};

#endif // FORMTESTSETTINGS_H
