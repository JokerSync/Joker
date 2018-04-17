/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "AboutDialog.h"
#include "ui_AboutDialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutDialog)
{
	ui->setupUi(this);

	setText(0);

	QPixmap pix(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/joker.png");
	ui->iconLabel->setPixmap(pix);
	ui->iconLabel->setScaledContents(true);
}

AboutDialog::~AboutDialog()
{
	delete ui;
}

void AboutDialog::setTimePlayed(PhTime timePlayed)
{
	setText(timePlayed);
}

void AboutDialog::on_pushButton_clicked()
{
	this->close();
}

void AboutDialog::setText(PhTime timePlayed)
{
	QString branch = PH_GIT_BRANCH;
	QString aboutMessage = tr("<p>Joker v%1 (build %2%3%4)</p>"
	                          "<p>Revision: <a href=\"https://github.com/jokersync/joker/tree/%5\">%6</a></p>"
	                          "<p>Report issues on <a href=\"https://github.com/jokersync/joker/issues\">GitHub</a></p>"
	                          "<p>Copyright 2013-2016 Phonations, All rights reserved.</p>"
	                          "<p>Joker is based on the following libraries:</p>"
	                          "<ul>"
	                          "<li><a href=\"http://www.opengl.org/\">OpenGL</a></li>"
	                          "<li><a href=\"http://ffmpeg.org/\">FFMPEG</a></li>"
	                          "</ul>"
	                          "<p>Synchronous playback time: %7 seconds</p>")
	                       .arg(PH_VERSION)
	                       .arg(PH_BUILD)
	                       .arg(QString(PH_FULL_VERSION).contains("dirty") ? "*" : "")
	                       .arg(branch == "master" ? "" : " " + branch)
	                       .arg(PH_GIT_REVISION)
	                       .arg(QString(PH_GIT_REVISION).left(7))
	                       .arg(timePlayed / PHTIMEBASE);

	ui->label->setText(aboutMessage);
}
