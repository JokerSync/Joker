/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include "PhCommonUI/PhUI.h"
#include "PhStrip/PhStripDoc.h"
#include "PhVideo/PhVideoEngine.h"

namespace Ui {
class PropertyDialog;
}

/**
 * @brief Show some properties of the strip file and the matching video.
 */
class PropertyDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * @brief The PropertyDialog constructor
	 * @param parent The parent object
	 */
	explicit PropertyDialog(QWidget *parent = 0);
	~PropertyDialog();

	/**
	 * @brief Set the document
	 *
	 * Attach the given doc to the window in order to access informations.
	 *
	 * @param doc The application document
	 */
	void setDoc(PhStripDoc * doc);
	/**
	 * @brief Set the video engine
	 *
	 * Attach the given video engine to the window in order to access informations.
	 *
	 * @param videoEngine The application video engine
	 */
	void setVideoEngine(PhVideoEngine *videoEngine);

protected:
	/**
	 * @brief Called when the dialog is shown
	 *
	 * Set up the dialog with the revelent informations.
	 */
	void showEvent(QShowEvent *);

private:
	Ui::PropertyDialog *ui;
	PhStripDoc *_doc;
	PhVideoEngine *_videoEngine;
};

#endif // PROPERTYDIALOG_H
