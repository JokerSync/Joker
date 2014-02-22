#ifndef PHDOCUMENTWINDOWSETTINGS_H
#define PHDOCUMENTWINDOWSETTINGS_H

#include <QString>

class PhDocumentWindowSettings
{
public:
	virtual QString currentDocument() = 0;
	virtual void setCurrentDocument(QString currentDocument) = 0;

	virtual QString lastDocumentFolder() = 0;
	virtual void setLastDocumentFolder(QString lastDocumentFolder) = 0;
};

#endif // PHDOCUMENTWINDOWSETTINGS_H
