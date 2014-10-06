#include <QFileInfo>
#include <QMessageBox>
#include <QDir>

#include "PhTools/PhDebug.h"

#include "PhDocumentWindow.h"

PhDocumentWindow::PhDocumentWindow(PhDocumentWindowSettings *settings)
	: PhWindow(settings),
	_settings(settings)
{
	connect(&_watcher, SIGNAL(fileChanged(QString)), this, SLOT(onExternalChange(QString)));
}

void PhDocumentWindow::processArg(int argc, char *argv[])
{
#warning /// @todo move to PhApplication

	for(int i = 1; i < argc; i++) {
		if(QFile::exists(argv[i]))
			_settings->setCurrentDocument(argv[i]);
	}
	if(QFile::exists(_settings->currentDocument()))
		openDocument(_settings->currentDocument());
	else
		updateRecentDocumentMenu();
}

void PhDocumentWindow::resetDocument()
{
	_settings->setCurrentDocument("");
	if(!_watcher.files().isEmpty())
		_watcher.removePaths(_watcher.files());
}

bool PhDocumentWindow::openDocument(const QString &fileName)
{
	resetDocument();
	if(_watcher.addPath(fileName))
		PHDEBUG << "now watching " << fileName;
	_settings->setCurrentDocument(fileName);
	if(!fileName.isEmpty())
		_settings->setLastDocumentFolder(QFileInfo(fileName).absolutePath());
	this->setWindowTitle(fileName);

	QStringList recentDocList = _settings->recentDocumentList();
	recentDocList.removeAll(fileName);
	recentDocList.insert(0, fileName);
	while(recentDocList.size() > _settings->maxRecentDocument())
		recentDocList.removeLast();

	_settings->setRecentDocumentList(recentDocList);

	updateRecentDocumentMenu();

	return true;
}

void PhDocumentWindow::onOpenRecentDocumentTriggered()
{
	QString fileName = sender()->objectName();
	PHDEBUG << fileName;
	openDocument(fileName);
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

void PhDocumentWindow::onExternalChange(const QString &path)
{
	PHDEBUG << "File changed :" << path;
	openDocument(_settings->currentDocument());
}
