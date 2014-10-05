#ifndef PHEDITABLEDOCUMENTWINDOW_H
#define PHEDITABLEDOCUMENTWINDOW_H

#include <QMenu>
#include <QFileSystemWatcher>
#include <QTime>

#include "PhDocumentWindow.h"

/**
 * @brief Common window behaviour for application handling document edition
 *
 * This class provide common user interface mechanism for application
 * handling document:
 * -
 */
class PhEditableDocumentWindow : public PhDocumentWindow
{
	Q_OBJECT
public:
	/**
	 * @brief PhEditableDocumentWindow constructor
	 * @param settings The document window settings
	 */
	explicit PhEditableDocumentWindow(PhDocumentWindowSettings *settings);

protected:
	/**
	 * @brief Check if the current document is modified and need to be save.
	 *
	 * If the document need to be saved, ask the user
	 * whether he wants to save his changes.
	 *
	 * @return False to interrupt the caller action, true otherwhise.
	 */
	virtual bool checkDocumentModification();

	virtual void on_actionSave_triggered() = 0;

	/**
	 * @brief Check if the current document has been modified.
	 * @return True if the document is modified, false otherwise.
	 */
	virtual bool isDocumentModified() = 0;

	virtual void closeEvent(QCloseEvent *event);
protected slots:
	void onOpenRecentDocumentTriggered() override;
};

#endif // PHEDITABLEDOCUMENTWINDOW_H
