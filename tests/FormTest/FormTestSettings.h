#ifndef FORMTESTSETTINGS_H
#define FORMTESTSETTINGS_H

#include <QDir>
#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class FormTestSettings : protected PhGenericSettings, public PhDocumentWindowSettings
{
public:
	PH_SETTING_STRING(CurrentDocument, currentDocument)
	PH_SETTING_STRING2(LastDocumentFolder, lastDocumentFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(RecentDocumentList, recentDocumentList)
};

#endif // FORMTESTSETTINGS_H
