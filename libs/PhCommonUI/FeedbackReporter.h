/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef FEEDBACKREPORTER_H
#define FEEDBACKREPORTER_H

#include <QWidget>

#include "PhTools/PhDebug.h"
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

private:
	Ui::FeedbackReporter *ui;
	PhFeedbackSettings *_settings;
	QString _lastLog;
};

#endif // FEEDBACKREPORTER_H
