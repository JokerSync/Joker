#include <QFileInfo>

#include "PhTools/PhDebug.h"

#include "PhDocumentWindow.h"

PhDocumentWindow::PhDocumentWindow(PhDocumentWindowSettings *settings, QWidget *parent)
	: QMainWindow(parent),
	  _settings(settings)
{
}

bool PhDocumentWindow::openFile(QString fileName)
{
	if(QFile::exists(fileName)) {
		_settings->setLastFile(fileName);
		_settings->setLastFolder(QFileInfo(fileName).absolutePath());
		return true;
	}
	return false;
}
