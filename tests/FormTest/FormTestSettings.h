#ifndef FORMTESTSETTINGS_H
#define FORMTESTSETTINGS_H

#include <QDir>
#include "PhTools/PhGenericSettings.h"
#include "PhCommonUI/PhDocumentWindowSettings.h"

class FormTestSettings : protected PhGenericSettings, public PhDocumentWindowSettings
{
public:
	PH_SETTING_STRING(LastFile, lastFile)
	PH_SETTING_STRING2(LastFolder, lastFolder, QDir::homePath())
	PH_SETTING_STRINGLIST(RecentDocuments, recentDocuments)
};

#endif // FORMTESTSETTINGS_H
