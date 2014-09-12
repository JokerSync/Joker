/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

#include "SonyToolSettings.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PreferencesDialog(QWidget *parent, SonyToolSettings *settings);
	~PreferencesDialog();

protected slots:
	/**
	 * @brief Update the settings according to the UI
	 */
	void accept();

private:

	Ui::PreferencesDialog *ui;

	SonyToolSettings *_settings;
};

#endif // PREFERENCESDIALOG_H
