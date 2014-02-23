#ifndef PHDOCUMENTWINDOW_H
#define PHDOCUMENTWINDOW_H

#include <QMainWindow>
#include <QMenu>

#include "PhDocumentWindowSettings.h"

class PhDocumentWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit PhDocumentWindow(PhDocumentWindowSettings *settings, QWidget *parent = 0);

	void processArg(int argc, char *argv[]);
protected:
	virtual bool openFile(QString fileName) = 0;
	void setCurrentDocument(QString fileName);
	virtual QMenu *recentDocumentMenu() { return NULL;}

private slots:
	void onOpenRecentDocumentTriggered();

private:
	void updateRecentDocumentMenu();
	PhDocumentWindowSettings * _settings;
};

#endif // PHDOCUMENTWINDOW_H
