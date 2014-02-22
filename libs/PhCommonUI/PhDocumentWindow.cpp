#include <QFileInfo>

#include "PhTools/PhDebug.h"

#include "PhDocumentWindow.h"

PhDocumentWindow::PhDocumentWindow(PhDocumentWindowSettings *settings, QWidget *parent)
	: QMainWindow(parent),
	_settings(settings)
{
}

void PhDocumentWindow::processArg(int argc, char *argv[])
{
	for(int i = 1; i < argc; i++) {
		if(QFile::exists(argv[i]))
			_settings->setCurrentDocument(argv[i]);
	}
	if(QFile::exists(_settings->currentDocument()))
		openFile(_settings->currentDocument());
}

bool PhDocumentWindow::openFile(QString fileName)
{
	if(QFile::exists(fileName)) {
		_settings->setCurrentDocument(fileName);
		_settings->setLastDocumentFolder(QFileInfo(fileName).absolutePath());
		this->setWindowTitle(fileName);
		return true;
	}
	return false;
}
