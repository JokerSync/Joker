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
	QString version = APP_NAME;
	version += " v";
	version += APP_VERSION;
	ui->labelVersion->setText(version);

	ui->graphicsViewIcon->setStyleSheet("background: transparent");

	QGraphicsScene *scn = new QGraphicsScene( ui->graphicsViewIcon );
	scn->setSceneRect( ui->graphicsViewIcon->rect() );
	ui->graphicsViewIcon->setScene( scn );
	ui->graphicsViewIcon->setFixedSize(ui->graphicsViewIcon->width(), ui->graphicsViewIcon->height() );
	QPixmap pix(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/joker.png");
	scn->addPixmap( pix );
}

AboutDialog::~AboutDialog()
{
	delete ui;
}

void AboutDialog::on_pushButton_clicked()
{
	this->close();
}
