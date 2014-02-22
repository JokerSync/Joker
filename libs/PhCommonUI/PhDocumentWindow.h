#ifndef PHDOCUMENTWINDOW_H
#define PHDOCUMENTWINDOW_H

#include <QMainWindow>

#include "PhDocumentWindowSettings.h"

class PhDocumentWindow : public QMainWindow
{
	Q_OBJECT
public:
	explicit PhDocumentWindow(PhDocumentWindowSettings *settings, QWidget *parent = 0);

	void processArg(int argc, char *argv[]);
protected:
	virtual bool openFile(QString fileName);

private:
	PhDocumentWindowSettings * _settings;
};

#endif // PHDOCUMENTWINDOW_H
