#ifndef PHDOCUMENTWINDOWSETTINGS_H
#define PHDOCUMENTWINDOWSETTINGS_H

#include <QString>

class PhDocumentWindowSettings
{
public:
	virtual QString lastFile() = 0;
	virtual void setLastFile(QString lastFile) = 0;

	virtual QString lastFolder() = 0;
	virtual void setLastFolder(QString lastFolder) = 0;
};

#endif // PHDOCUMENTWINDOWSETTINGS_H
