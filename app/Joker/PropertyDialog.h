/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PROPERTYDIALOG_H
#define PROPERTYDIALOG_H

#include <QDialog>
#include "PhStrip/PhStripDoc.h"
#include "PhVideo/PhVideoEngine.h"

namespace Ui {
class PropertyDialog;
}

/*!
 * \brief The PropertyDialog class
 * Show some properties of the strip file and the matching video
 */
class PropertyDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PropertyDialog(QWidget *parent = 0);
	~PropertyDialog();

	/*!
	 * \brief setDoc
	 * \param doc
	 *
	 * Attach the given doc to the window in order to access informations
	 */
	void setDoc(PhStripDoc * doc);
	/*!
	 * \brief setVideoEngine
	 * \param videoEngine
	 *
	 * Attach the given videoEngine to the window in order to access informations
	 */
	void setVideoEngine(PhVideoEngine *videoEngine);

protected:
	/*!
	 * \brief showEvent
	 * Set up the UI with the revelent informations.
	 */
	void showEvent(QShowEvent *);

private:
	Ui::PropertyDialog *ui;
	PhStripDoc *_doc;
	PhVideoEngine *_videoEngine;
};

#endif // PROPERTYDIALOG_H
