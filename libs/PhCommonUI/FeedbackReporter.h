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
#include "PhTools/SMTP/SmtpMime"

#include "PhFeedbackSettings.h"



namespace Ui {
class FeedbackReporter;
}

class FeedbackReporter : public QWidget
{
	Q_OBJECT

public:
	explicit FeedbackReporter(PhFeedbackSettings *settings, QWidget *parent = 0);
	~FeedbackReporter();

private slots:
	void on_buttonBox_accepted();

	void on_buttonBox_rejected();

	void onSyncRequestFinished(QNetworkReply*reply);

private:
	Ui::FeedbackReporter *ui;
	PhFeedbackSettings *_settings;
};

#endif // FEEDBACKREPORTER_H
