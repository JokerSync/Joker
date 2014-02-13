/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#include "AboutMenu.h"
#include "ui_AboutMenu.h"

AboutMenu::AboutMenu(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AboutMenu)
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

AboutMenu::~AboutMenu()
{
	delete ui;
}

void AboutMenu::on_pushButton_clicked()
{
	this->close();
}
