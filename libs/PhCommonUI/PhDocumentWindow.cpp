#include <QFileInfo>
#include <QMessageBox>

#include "PhTools/PhDebug.h"

#include "PhDocumentWindow.h"

PhDocumentWindow::PhDocumentWindow(PhDocumentWindowSettings *settings)
	: PhWindow(settings),
	_settings(settings)
{
	_restrain.start();
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

void PhDocumentWindow::setCurrentDocument(QString fileName)
{
	if(!_watcher.files().isEmpty())
		_watcher.removePaths(_watcher.files());
	if(_watcher.addPath(fileName))
		PHDEBUG << "now watching " << fileName;
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
#warning /// @todo check to save if needed
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
void PhDocumentWindow::onExternalChange(QString path)
{
	if(_restrain.restart() > 1000) {
		PHDEBUG << "File changed :" << path;
		int ret = QMessageBox::warning(this,
		                               "Warning",
		                               "The document has been modified outside of "
		                               APP_NAME
		                               ".\n"
		                               "Do you want to load the changes?",
		                               QMessageBox::Yes | QMessageBox::No,
		                               QMessageBox::Yes);
		if (ret == QMessageBox::Yes)
			openDocument(path, true);
		_restrain.restart();
	}
}
