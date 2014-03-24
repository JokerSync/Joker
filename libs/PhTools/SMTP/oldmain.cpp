///**
//* Copyright (C) 2012-2014 Phonations
//* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
//*/
//#include <QApplication>

//#include "SmtpMime"


//int main(int argc, char *argv[])
//{
//	QApplication a(argc, argv);

//	// This is a first demo application of the SmtpClient for Qt project


//	// First we need to create an SmtpClient object
//	// We will use the Gmail's smtp server (smtp.gmail.com, port 465, ssl)

//	SmtpClient smtp("smtp.googlemail.com", 465, SmtpClient::SslConnection);

//	// We need to set the username (your email address) and password
//	// for smtp authentification.

//	smtp.setUser("");
//	smtp.setPassword("");

//	// Now we create a MimeMessage object. This is the email.

//	MimeMessage message;

//	message.setSender(new EmailAddress("", "Code2"));
//	message.addRecipient(new EmailAddress("", "Client"));
//	message.setSubject("SmtpClient for Qt - Demo");
//	message.addPart(new MimeAttachment(new QFile("/Users/thomas/Pictures/ab5gVVE_460s.jpg")));


//	// Now add some text to the email.
//	// First we create a MimeText object.

//	MimeText text;

//	text.setText("Hi,\nThis is a simple email message.\n");

//	// Now add it to the mail

//	message.addPart(&text);



//	// Now we can send the mail
//	if(smtp.connectToHost())
//		qDebug() << "host reached.";
//	if(smtp.login())
//		qDebug() << "Login succeed.";
//	if(smtp.sendMail(message))
//		qDebug() << "Message sent.";
//	smtp.quit();

//}
