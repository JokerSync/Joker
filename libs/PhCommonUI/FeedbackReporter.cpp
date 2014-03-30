/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QDir>
#include <QDateTime>
#include <QSettings>
#include <QMessageBox>
#include <QProcess>

#include "FeedbackReporter.h"
#include "ui_FeedbackReporter.h"


FeedbackReporter::FeedbackReporter(PhFeedbackSettings *settings, QWidget *parent) :
	QWidget(parent),
	_settings(settings),
	ui(new Ui::FeedbackReporter)
{

	ui->setupUi(this);

	ui->labelTitle->setText(QString(APP_NAME) + " " +tr("recently quit unexpectedly."));

	PHDEBUG << _settings->emailList();
	ui->comboBoxEmails->addItems(_settings->emailList());
}

FeedbackReporter::~FeedbackReporter()
{
	delete ui;
}

void FeedbackReporter::on_buttonBox_accepted()
{
	QStringList emails;
	for(int i = 0; i < ui->comboBoxEmails->count(); i++) {
		emails += ui->comboBoxEmails->itemText(i);
	}
	emails.removeOne(ui->comboBoxEmails->currentText());
	emails.insert(0, ui->comboBoxEmails->currentText());
	_settings->setEmailList(emails);



	QString systemConfig;
	QString crashLog;
	QString appLog;
	QString header;
	QString preferences;


	header = "";

	header += "--------Feedback report: " + QString(APP_NAME) + " v" + QString(APP_VERSION) + "--------\n";
	header += "From : " + ui->comboBoxEmails->currentText() + "\n";
	if(!ui->textEditComment->toPlainText().isEmpty())
		header += "Message : " + ui->textEditComment->toPlainText() + "\n";


	// Get the system infos
	if(ui->checkBoxFile3->isChecked()) {
		system("/usr/sbin/system_profiler SPHardwareDataType > out");
		QFile file("./out");
		if(!file.open(QIODevice::ReadOnly)) {
			PHDEBUG << file.errorString();
		}
		else {
			QTextStream in(&file);
			while(!in.atEnd()) {
				systemConfig += in.readLine() + "\n";
			}
			file.close();
			system("rm out");
		}
	}

	// Get the preferences
	if(ui->checkBoxFile2->isChecked()) {
		QString cmd = "defaults read com.Phonations." + QString(APP_NAME) + " > out";
		system(PHNQ(cmd));
		QFile file("./out");
		if(!file.open(QIODevice::ReadOnly)) {
			PHDEBUG << file.errorString();
		}
		else {
			QTextStream in(&file);
			while(!in.atEnd()) {
				preferences += in.readLine() + "\n";
			}
			file.close();
			system("rm out");
		}
	}

	// Get the application log
	if(ui->checkBoxFile4->isChecked()) {
		QFile file(QDir::homePath() + "/Library/Logs/Phonations/" + APP_NAME + ".log");
		if(!file.open(QIODevice::ReadOnly)) {
			PHDEBUG << file.errorString();
		}
		else {
			QTextStream in(&file);
			while(!in.atEnd()) {
				appLog += in.readLine()  + "\n";
			}
			file.close();
		}
	}

	// Get the crash log
	if(ui->checkBoxFile1->isChecked()) {
		QString userDirectoryLogs = QDir::homePath() + "/Library/Logs/DiagnosticReports/";
		QStringList files;
		QDir logs(userDirectoryLogs);

		QStringList filters;
		filters.append(QString(APP_NAME) + "*.crash" );
		logs.setNameFilters(filters);
		files = logs.entryList();
		QString lastCrashLog = (files.first());
		foreach(QString file, files)
		{
			if(QFileInfo(file).created() > QFileInfo(lastCrashLog).created())
				lastCrashLog = file;
		}
		QFile file(lastCrashLog);
		if(!file.open(QIODevice::ReadOnly)) {
			PHDEBUG << "Crash log : " << file.errorString();
		}
		else {
			QTextStream in(&file);
			while(!in.atEnd()) {
				crashLog += in.readLine()  + "\n";
			}
			file.close();
		}
	}

	QString name;
	// Get the machine name
	system("scutil --get ComputerName > out");
	QFile file("./out");
	if(!file.open(QIODevice::ReadOnly)) {
		PHDEBUG << file.errorString();
	}
	else {
		QTextStream in(&file);
		while(!in.atEnd()) {
			name += in.readLine() + "\n";
		}
		file.close();
		system("rm out");
	}

	name.remove("[=|&]");
	name.insert(0, "name=");
	name.append("&");

	header.remove("[=|&]");
	header.insert(0, "header=");
	header.append("&");

	QByteArray post;

	post = name.toUtf8() + header.toUtf8();

	if(!preferences.isEmpty()) {
		preferences.remove("[=|&]");
		preferences.insert(0, "preferences=");
		preferences.append("&");
		post += preferences.toUtf8();
		PHDEBUG << "add preferences";
	}

	if(!systemConfig.isEmpty()) {
		systemConfig.remove("[=|&]");
		systemConfig.insert(0, "configuration=");
		systemConfig.append("&");
		post += systemConfig.toUtf8();
		PHDEBUG << "add systemConfig";
	}

	if(!appLog.isEmpty()) {
		appLog.replace("&", "amp");
		appLog.insert(0, "applicationLog=");
		appLog.append("&");
		post += appLog.toUtf8();
		PHDEBUG << "add appLog";
	}

	if(!crashLog.isEmpty()) {
		crashLog.remove("[=|&]");
		crashLog.insert(0, "crashLog=");
		crashLog.append("&");
		post += crashLog.toUtf8();
		PHDEBUG << "add crashLog";
	}

	QNetworkRequest request(QUrl("http://feedback.phonations.com/submit.php"));
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onSyncRequestFinished(QNetworkReply*)));

	// Send it
	manager->post(request, post);
	QMessageBox::information(this, "Information",
	                         tr("Thank you for your feedback!"),
	                         QMessageBox::Ok, QMessageBox::Ok);
	hide();
}

void FeedbackReporter::on_buttonBox_rejected()
{
	hide();
}

void FeedbackReporter::onSyncRequestFinished(QNetworkReply * reply)
{
	delete reply;
}
