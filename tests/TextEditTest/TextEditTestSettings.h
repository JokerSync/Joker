#ifndef TEXTEDITTESTSETTINGS_H
#define TEXTEDITTESTSETTINGS_H

#include <QDir>

#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class TextEditTestSettings : protected PhGenericSettings, public PhDocumentWindowSettings
{
public:
	PH_SETTING_STRING(setCurrentDocument, currentDocument)
	PH_SETTING_STRING2(setLastDocumentFolder, lastDocumentFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(setRecentDocumentList, recentDocumentList)

	PH_SETTING_INT2(setMaxRecentDocument, maxRecentDocument, 3)
};

#endif // TEXTEDITTESTSETTINGS_H
