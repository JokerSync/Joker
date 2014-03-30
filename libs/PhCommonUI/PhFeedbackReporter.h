/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef FEEDBACKREPORTER_H
#define FEEDBACKREPORTER_H

#include <QWidget>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonObject>
#include <QNetworkReply>

#include "PhTools/PhDebug.h"

#include "PhFeedbackSettings.h"



namespace Ui {
class PhFeedbackReporter;
}

/**
 * @brief The FeedbackReporter class
 *
 * This class allow the user to send crash
 * report, logs and informations about the
 * machine configuration.
 */
class PhFeedbackReporter : public QWidget
{
	Q_OBJECT

public:
	/**
	 * @brief FeedbackReporter
	 *
	 * Default constructor of the feedback module,
	 * You MUST provide settings to make it work
	 * normally.
	 *
	 * @param settings
	 * @param parent
	 */
	explicit PhFeedbackReporter(PhFeedbackSettings *settings, QWidget *parent = 0);
	~PhFeedbackReporter();

private slots:
	void on_buttonBox_accepted();

	void on_buttonBox_rejected();

	void onSyncRequestFinished(QNetworkReply*reply);

private:
	Ui::PhFeedbackReporter *ui;
	PhFeedbackSettings *_settings;
};

#endif // FEEDBACKREPORTER_H
