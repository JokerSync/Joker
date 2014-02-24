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
		openDocument(_settings->currentDocument());
	else
		updateRecentDocumentMenu();
}

void PhDocumentWindow::setCurrentDocument(QString fileName)
{
	_settings->setCurrentDocument(fileName);
	_settings->setLastDocumentFolder(QFileInfo(fileName).absolutePath());
	this->setWindowTitle(fileName);

	QStringList recentDocList = _settings->recentDocumentList();
	while(recentDocList.contains(fileName))
		recentDocList.removeOne(fileName);
	recentDocList.insert(0, fileName);
	while(recentDocList.size() > _settings->maxRecentDocument())
		recentDocList.removeLast();

	_settings->setRecentDocumentList(recentDocList);

	updateRecentDocumentMenu();
}

void PhDocumentWindow::onOpenRecentDocumentTriggered()
{
#warning TODO check to save if needed
	openDocument(sender()->objectName());
}

void PhDocumentWindow::updateRecentDocumentMenu()
{
	if(recentDocumentMenu()) {
		QStringList recentDocList = _settings->recentDocumentList();

		if(_settings->recentDocumentList().count()) {
			recentDocumentMenu()->clear();
			foreach(QString doc, recentDocList) {
				QAction *action = recentDocumentMenu()->addAction(doc);
				action->setObjectName(doc);
				connect(action, SIGNAL(triggered()), this, SLOT(onOpenRecentDocumentTriggered()));
			}
		}
	}
}
