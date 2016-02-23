/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef WINDOWDOCUMENTWINDOWSPECSETTINGS_H
#define WINDOWDOCUMENTWINDOWSPECSETTINGS_H

#include <QDir>

#include "PhCommonUI/PhDocumentWindowSettings.h"

#include "PhTools/PhGenericSettings.h"

class DocumentWindowSpecSettings : public PhGenericSettings, public PhDocumentWindowSettings
{
public:
	DocumentWindowSpecSettings() : PhGenericSettings(true) {
	}

	// PhWindowSettings
	PH_SETTING_BYTEARRAY(setWindowGeometry, windowGeometry)

	// PhDocumentWindowSettings
	PH_SETTING_STRING(setCurrentDocument, currentDocument)
	PH_SETTING_STRING2(setLastDocumentFolder, lastDocumentFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(setRecentDocumentList, recentDocumentList)
	PH_SETTING_INT2(setMaxRecentDocument, maxRecentDocument, 5)
	PH_SETTING_BOOL2(setAutoReload, autoReload, true)
	PH_SETTING_HASH(setBookmark, bookmark)
};

#endif // WINDOWDOCUMENTWINDOWSPECSETTINGS_H
