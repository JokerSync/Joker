#ifndef PHDOCUMENTWINDOWSETTINGS_H
#define PHDOCUMENTWINDOWSETTINGS_H

#include "PhWindowSettings.h"

#include <QByteArray>
#include <QVariant>

/**
 * @brief The settings for PhDocumentWindow
 */
class PhDocumentWindowSettings : public PhWindowSettings
{
public:
	/**
	 * @brief Get the current document
	 * @return A file name
	 */
	virtual QString currentDocument() = 0;
	/**
	 * @brief Set the current document
	 * @param currentDocument A file name
	 */
	virtual void setCurrentDocument(QString currentDocument) = 0;

	/**
	 * @brief Get the last document folder
	 * @return A folder path
	 */
	virtual QString lastDocumentFolder() = 0;
	/**
	 * @brief Set the last document folder
	 * @param lastDocumentFolder A folder path
	 */
	virtual void setLastDocumentFolder(QString lastDocumentFolder) = 0;

	/**
	 * @brief Get the recent document list
	 * @return A list of file name
	 */
	virtual QStringList recentDocumentList() = 0;
	/**
	 * @brief Set the recent document list
	 * @param list A list of file name
	 */
	virtual void setRecentDocumentList(QStringList list) = 0;

	/**
	 * @brief Get the maximun number of recent document
	 * @return A number of document
	 */
	virtual int maxRecentDocument() = 0;

	/**
	 * @brief Get the file auto reload property
	 * @return True if autoreload enabled, false otherwise
	 */
	virtual bool autoReload() = 0;

	/**
	 * @brief Get the security scope bookmark of a file
	 * @param fileName A file name
	 * @return A QByteArray containing the bookmark
	 */
	virtual QVariant bookmark(QString fileName) = 0;

	/**
	 * @brief Set the security scope bookmark of a file
	 * @param fileName A file name
	 * @param bookmark A QByteArray containing the bookmark
	 */
	virtual void setBookmark(QString fileName, QVariant bookmark) = 0;
};

#endif // PHDOCUMENTWINDOWSETTINGS_H
