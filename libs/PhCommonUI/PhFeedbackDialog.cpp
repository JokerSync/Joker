/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QDir>
#include <QDateTime>
#include <QSettings>
#include <QMessageBox>
#include <QProcess>

#include "PhFeedbackDialog.h"
#include "ui_PhFeedbackDialog.h"


PhFeedbackDialog::PhFeedbackDialog(PhFeedbackSettings *settings, QWidget *parent) :
	QDialog(parent),
	_settings(settings),
	ui(new Ui::PhFeedbackDialog)
{

	ui->setupUi(this);

	ui->problemLabel->setText(ui->problemLabel->text().arg(QString(APP_NAME)));

	PHDEBUG << _settings->emailList();
	ui->comboBoxEmails->addItems(_settings->emailList());
}

PhFeedbackDialog::~PhFeedbackDialog()
{
	delete ui;
}

void PhFeedbackDialog::on_buttonBox_accepted()
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
	system("/usr/sbin/system_profiler SPHardwareDataType > out");
	QFile file1("./out");
	if(!file1.open(QIODevice::ReadOnly))
		PHDEBUG << file1.errorString();
	else {
		QTextStream in(&file1);
		while(!in.atEnd()) {
			systemConfig += in.readLine() + "\n";
		}
		file1.close();
		system("rm out");
	}


	// Get the preferences
	QString cmd = "defaults read com.Phonations." + QString(APP_NAME) + " > out";
	system(PHNQ(cmd));
	QFile file2("./out");
	if(!file2.open(QIODevice::ReadOnly)) {
		PHDEBUG << file2.errorString();
	}
	else {
		QTextStream in(&file2);
		while(!in.atEnd()) {
			preferences += in.readLine() + "\n";
		}
		file2.close();
		system("rm out");
	}


	// Get the application log
	QFile file3(QDir::homePath() + "/Library/Logs/Phonations/" + APP_NAME + ".log");
	if(!file3.open(QIODevice::ReadOnly)) {
		PHDEBUG << file3.errorString();
	}
	else {
		QTextStream in(&file3);
		while(!in.atEnd()) {
			appLog += in.readLine()  + "\n";
		}
		// Stripping only the end of the log
		if(appLog.length() > 10000)
			appLog = appLog.mid(appLog.length() - 10000);
		file3.close();
	}


	// Get the crash log
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
	QFile file4(lastCrashLog);
	if(!file4.open(QIODevice::ReadOnly)) {
		PHDEBUG << "Crash log : " << file4.errorString();
	}
	else {
		QTextStream in(&file4);
		while(!in.atEnd()) {
			crashLog += in.readLine()  + "\n";
		}
		file4.close();
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

	QString post;

	post = name + header;

	if(!preferences.isEmpty()) {
		preferences.remove("[=|&]");
		preferences.insert(0, "preferences=");
		preferences.append("&");
		post += preferences;
		PHDEBUG << "add preferences";
	}

	if(!systemConfig.isEmpty()) {
		systemConfig.remove("[=|&]");
		systemConfig.insert(0, "configuration=");
		systemConfig.append("&");
		post += systemConfig;
		PHDEBUG << "add systemConfig";
	}

	if(!appLog.isEmpty()) {
		appLog.replace("&", "amp");
		appLog.insert(0, "applicationLog=");
		appLog.append("&");
		post += appLog;
		PHDEBUG << "add appLog";
	}

	if(!crashLog.isEmpty()) {
		crashLog.remove("[=|&]");
		crashLog.insert(0, "crashLog=");
		crashLog.append("&");
		post += crashLog;
		PHDEBUG << "add crashLog";
	}

	QNetworkRequest request(QUrl("http://www.phonations.com/feedback.php"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onSyncRequestFinished(QNetworkReply*)));

	// Send it
	manager->post(request, post.toUtf8());
	QMessageBox::information(this, "Information",
							 tr("Thank you for your feedback!"),
							 QMessageBox::Ok, QMessageBox::Ok);
	hide();
}

void PhFeedbackDialog::on_buttonBox_rejected()
{
	hide();
}

void PhFeedbackDialog::onSyncRequestFinished(QNetworkReply * reply)
{
	delete reply;
}
