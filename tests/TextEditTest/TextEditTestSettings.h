#ifndef TEXTEDITTESTSETTINGS_H
#define TEXTEDITTESTSETTINGS_H

#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class TextEditTestSettings : protected PhGenericSettings, public PhDocumentWindowSettings
{
public:
	PH_SETTING_STRING(CurrentDocument, currentDocument)
	PH_SETTING_STRING(LastDocumentFolder, lastDocumentFolder)
	PH_SETTING_STRINGLIST(RecentDocumentList, recentDocumentList)

	PH_SETTING_INT2(MaxRecentDocument, maxRecentDocument, 3)
};

#endif // TEXTEDITTESTSETTINGS_H
