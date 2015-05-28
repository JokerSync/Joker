#include "PhTools/PhDebug.h"

#include "PhEditableDocumentWindow.h"

PhEditableDocumentWindow::PhEditableDocumentWindow(PhDocumentWindowSettings *settings)
	: PhDocumentWindow(settings),
	_settings(settings)
{
}

bool PhEditableDocumentWindow::openDocument(const QString &fileName)
{
	if(checkDocumentModification())
		return PhDocumentWindow::openDocument(fileName);
	return false;
}

bool PhEditableDocumentWindow::saveDocument(const QString &fileName)
{
	_settings->setCurrentDocument(fileName);
	this->setWindowTitle(fileName);
	return true;
}

bool PhEditableDocumentWindow::checkDocumentModification()
{
	if(isDocumentModified()) {
		QString msg = tr("Do you want to save your changes ?");
		QMessageBox box(QMessageBox::Question, "", msg, QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
		box.setDefaultButton(QMessageBox::Save);
		switch(box.exec()) {
		/// Cancel the caller action if clicking cancel.
		case QMessageBox::Cancel:
			return false;
		/// Trigger the document save if clicking save:
		case QMessageBox::Save:
			on_actionSave_triggered();
			if(isDocumentModified())
				return false;
			break;
		}
	}
	return true;
}

void PhEditableDocumentWindow::closeEvent(QCloseEvent *event)
{
	/// Check if the current document has to be saved (it might cancel the action).
	if(!checkDocumentModification())
		event->ignore();
}

void PhEditableDocumentWindow::onOpenRecentDocumentTriggered()
{
	QString fileName = sender()->objectName();
	PHDEBUG << fileName;
	openDocument(fileName);
}
