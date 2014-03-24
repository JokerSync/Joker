/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include <QDir>
#include <QDateTime>
#include <QSettings>
#include <QMessageBox>

#include "FeedbackReporter.h"
#include "ui_FeedbackReporter.h"

//#define PASSWORD
//#define SENDER

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
	for(int i = 0; i < ui->comboBoxEmails->count(); i++)
	{
		emails += ui->comboBoxEmails->itemText(i);
	}
	emails.removeOne(ui->comboBoxEmails->currentText());
	emails.insert(0, ui->comboBoxEmails->currentText());
	_settings->setEmailList(emails);

	PHDEBUG << emails;

	SmtpClient smtp("smtp.googlemail.com", 465, SmtpClient::SslConnection);

#ifdef SENDER and PASSWORD
	smtp.setUser(SENDER);
	smtp.setPassword(PASSWORD);
#endif

	// Now we create a MimeMessage object. This is the email.

	MimeMessage message;

	message.setSender(new EmailAddress(ui->comboBoxEmails->currentText(), "Bug report"));
	message.addRecipient(new EmailAddress("support@phonations.com", "Support"));
	message.setSubject(tr("Bug repport"));

	MimeText text;

	text.setText("Hi Phonations,\nThis is report bug message form " + ui->comboBoxEmails->currentText() + ".\n");

	// Now add it to the mail
	if(!ui->textEditComment->toPlainText().isEmpty())
	{
		text.setText(text.getText() + "\nThe client added thoses infos : \"\n" + ui->textEditComment->toPlainText() + "\"");
	}
	message.addPart(&text);

	if(ui->checkBoxFile1->isChecked())
	{
		message.addPart(new MimeAttachment(new QFile(ui->labelFile1->text())));
		PHDEBUG << "send : " << ui->labelFile1->text();
	}
	if(ui->checkBoxFile2->isChecked())
	{
		message.addPart(new MimeAttachment(new QFile(ui->labelFile2->text())));
		PHDEBUG << "send : " << ui->labelFile2->text();
	}


	// Now we can send the mail
	if(smtp.connectToHost())
		qDebug() << "host reached.";
	if(smtp.login())
		qDebug() << "Login succeed.";
	bool succeed = smtp.sendMail(message);
	if (succeed)
		qDebug() << "Message sent.";
	smtp.quit();
	if(succeed)
		this->hide();
	else
		QMessageBox::warning(this, "Error", tr("Error while sending the mail"), QMessageBox::Ok);
}

void FeedbackReporter::on_buttonBox_rejected()
{
	this->hide();
}
