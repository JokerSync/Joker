#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	_stripView = ui->stripView;
	_doc = _stripView->doc();
	_clock = _stripView->clock();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openFile(QString fileName)
{
	qDebug() << "openFile : " << fileName;
  //  PhString fileName = QFileDialog::getOpenFileName(this, tr("Open a script"),QDir::homePath(), "Script File (*.detx)");
	if(QFile::exists(fileName))
	{
		if(_doc->openDetX(fileName))
		{
			_clock->setFrame(_doc->getLastFrame(), _doc->getTCType());
		}
	}
}
