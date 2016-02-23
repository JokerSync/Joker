#ifndef PHEDITABLEDOCUMENTWINDOW_H
#define PHEDITABLEDOCUMENTWINDOW_H

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
	bool openDocument(const QString &fileName) override;

	/**
	 * @brief Update the settings with the document name
	 * @param fileName A string.
	 * @return Always true.
	 */
	virtual void saveDocument(const QString &fileName);

	bool checkDocumentModification();

	/**
	 * @brief Virtual slot triggered when File/Save is clicked
	 */
	virtual void on_actionSave_triggered() = 0;

	/**
	 * @brief Check if the current document has been modified.
	 * @return True if the document is modified, false otherwise.
	 */
	virtual bool isDocumentModified() = 0;

	/**
	 * @brief Check if the document is modified and
	 * @param event
	 */
	virtual void closeEvent(QCloseEvent *event);

private:
	PhDocumentWindowSettings *_settings;
};

#endif // PHEDITABLEDOCUMENTWINDOW_H
