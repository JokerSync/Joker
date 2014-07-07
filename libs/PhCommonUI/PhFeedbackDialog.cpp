/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QProcess>
#include <QHostInfo>

#include "PhFeedbackDialog.h"
#include "ui_PhFeedbackDialog.h"


PhFeedbackDialog::PhFeedbackDialog(PhFeedbackSettings *settings, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::PhFeedbackDialog),
	_settings(settings)
{

	ui->setupUi(this);

	ui->problemLabel->setText(ui->problemLabel->text().arg(QString(APP_NAME)));

	if(!_settings->emailList().isEmpty())
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
	QString tempFileName(tmpnam(NULL));
#if defined(Q_OS_MAC)
	system(PHNQ(QString("/usr/sbin/system_profiler SPHardwareDataType > %1").arg(tempFileName)));
	QFile systemInfoFile(tempFileName);
	if(!systemInfoFile.open(QIODevice::ReadOnly))
		PHDEBUG << systemInfoFile.errorString();
	else {
		QTextStream in(&systemInfoFile);
		while(!in.atEnd()) {
			systemConfig += in.readLine() + "\n";
		}
		systemInfoFile.close();
		system(PHNQ(QString("rm %1").arg(tempFileName)));
	}
#else
	system(PHNQ(QString("rm %1").arg(tempFileName)));
#endif

	// Get the preferences
	system(PHNQ(QString("defaults read com.Phonations.%1 > %2").arg(QString(APP_NAME)).arg(tempFileName)));
	QFile preferencesFile(tempFileName);
	if(!preferencesFile.open(QIODevice::ReadOnly)) {
		PHDEBUG << preferencesFile.errorString();
	}
	else {
		QTextStream in(&preferencesFile);
		while(!in.atEnd()) {
			preferences += in.readLine() + "\n";
		}
		preferencesFile.close();
		system(PHNQ(QString("rm %1").arg(tempFileName)));
	}


	// Get the application log
	QFile applicationLogFile(QDir::homePath() + "/Library/Logs/Phonations/" + APP_NAME + ".log");
	if(!applicationLogFile.open(QIODevice::ReadOnly)) {
		PHDEBUG << applicationLogFile.errorString();
	}
	else {
		QTextStream in(&applicationLogFile);
		while(!in.atEnd()) {
			appLog += in.readLine()  + "\n";
		}
		// Stripping only the end of the log
		if(appLog.length() > 10000)
			appLog = appLog.mid(appLog.length() - 10000);
		applicationLogFile.close();
	}


	// Get the crash log
	QString crashFolder = QDir::homePath() + "/Library/Logs/DiagnosticReports/";
	QDir crashDir(crashFolder);

	QStringList crashFilters;
	crashFilters.append(QString(APP_NAME) + "*.crash" );
	crashDir.setNameFilters(crashFilters);
	QStringList crashFiles = crashDir.entryList();
	QString lastCrashFilePath = crashFolder + crashFiles.first();
	PHDEBUG << "last crash log:" << lastCrashFilePath;
	foreach(QString file, crashFiles)
	{
		QString filePath = crashFolder + file;
		if(QFileInfo(filePath).created() > QFileInfo(lastCrashFilePath).created())
			lastCrashFilePath = filePath;
	}
	PHDEBUG << lastCrashFilePath;
	QFile crashFile(lastCrashFilePath);
	if(!crashFile.open(QIODevice::ReadOnly)) {
		PHDEBUG << "Crash log : " << crashFile.errorString();
	}
	else {
		QTextStream in(&crashFile);
		while(!in.atEnd()) {
			crashLog += in.readLine()  + "\n";
		}
		crashFile.close();
	}


	QString name = QString("name=%1&").arg(QHostInfo::localHostName());

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
		PHDEBUG << "add preferences:" << preferences.length();
	}

	if(!systemConfig.isEmpty()) {
		systemConfig.remove("[=|&]");
		systemConfig.insert(0, "configuration=");
		systemConfig.append("&");
		post += systemConfig;
		PHDEBUG << "add systemConfig:" << systemConfig.length();
	}

	if(!appLog.isEmpty()) {
		appLog.replace("&", "amp");
		appLog.insert(0, "applicationLog=");
		appLog.append("&");
		post += appLog;
		PHDEBUG << "add appLog:" << appLog.length();
	}

	if(!crashLog.isEmpty()) {
		crashLog.remove("[=|&]");
		crashLog.insert(0, "crashLog=");
		crashLog.append("&");
		post += crashLog;
		PHDEBUG << "add crashLog:" << crashLog.length();
	}

	QNetworkRequest request(QUrl("http://www.phonations.com/feedback.php"));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onSyncRequestFinished(QNetworkReply*)));

	// Send it
	manager->post(request, post.toUtf8());
	QMessageBox::information(this, "Information", tr("Thank you for your feedback!"), QMessageBox::Ok, QMessageBox::Ok);
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
