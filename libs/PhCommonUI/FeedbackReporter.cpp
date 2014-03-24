/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QDir>
#include <QDateTime>
#include <QSettings>

#include "FeedbackReporter.h"
#include "ui_FeedbackReporter.h"

FeedbackReporter::FeedbackReporter(PhFeedbackSettings *settings, QWidget *parent) :
	QWidget(parent),
	_settings(settings),
	ui(new Ui::FeedbackReporter)
{

	ui->setupUi(this);

	ui->labelTitle->setText(QString(APP_NAME) + " " +tr("recently quit unexpectedly."));

	QString userDirectoryLogs = QDir::homePath() + "/Library/Logs/DiagnosticReports/";
	QStringList files;

	QDir logs(userDirectoryLogs);

	QStringList filters;
	QString s1 = "VideoTest";
	QString s2 = "*.crash";
	filters.append(s1 + s2);
	logs.setNameFilters(filters);
	files = logs.entryList();

	_lastLog = (files.first());
	foreach(QString file, files)
	{
		if(QFileInfo(file).created() > QFileInfo(_lastLog).created())
			_lastLog = file;
	}
	_lastLog = QDir::homePath() + "/Library/Logs/DiagnosticReports/" + _lastLog;
	ui->labelFile1->setText(QFileInfo(_lastLog).fileName());
	ui->labelFile2->setText(QFileInfo(_settings->settingsFileName()).fileName());
	ui->labelFile1->adjustSize();
	ui->labelFile2->adjustSize();

}

FeedbackReporter::~FeedbackReporter()
{
	delete ui;
}

void FeedbackReporter::on_buttonBox_accepted()
{

}
