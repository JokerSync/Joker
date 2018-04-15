/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef FEEDBACKDIALOG_H
#define FEEDBACKDIALOG_H

#include <QDialog>

#include "PhTools/PhDebug.h"


namespace Ui {
class PhFeedbackDialog;
}

/**
 * @brief The FeedbackDialog class
 *
 * This class guides the user to send crash
 * report, logs and informations about the
 * machine configuration to the GitHub issues.
 */
class PhFeedbackDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * @brief Feedback dialog
	 * @param parent
	 */
	explicit PhFeedbackDialog(QWidget *parent = 0);
	~PhFeedbackDialog();

private:
	Ui::PhFeedbackDialog *ui;
};

#endif // FEEDBACKDIALOG_H
