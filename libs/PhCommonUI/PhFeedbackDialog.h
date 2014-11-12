/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef FEEDBACKDIALOG_H
#define FEEDBACKDIALOG_H

#include "PhTools/PhGeneric.h"
#include "PhCommonUI/PhUI.h"

#include "PhTools/PhDebug.h"

#include "PhFeedbackSettings.h"



namespace Ui {
class PhFeedbackDialog;
}

/**
 * @brief The FeedbackDialog class
 *
 * This class allow the user to send crash
 * report, logs and informations about the
 * machine configuration.
 */
class PhFeedbackDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * @brief FeedbackDialog
	 *
	 * Default constructor of the feedback module,
	 * You MUST provide settings to make it work
	 * normally.
	 *
	 * @param settings
	 * @param parent
	 */
	explicit PhFeedbackDialog(PhFeedbackSettings *settings, QWidget *parent = 0);
	~PhFeedbackDialog();

private slots:
	void on_buttonBox_accepted();

	void on_buttonBox_rejected();

	void onSyncRequestFinished(QNetworkReply*reply);

private:
	Ui::PhFeedbackDialog *ui;
	PhFeedbackSettings *_settings;
};

#endif // FEEDBACKDIALOG_H
