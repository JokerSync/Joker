#include "MainView.h"
#include "ui_MainView.h"

#include <QPushButton>
#include <QBoxLayout>
#include <QFileInfo>
#include <QFileDialog>

MainView::MainView()
	: QMainWindow(0),
	  ui(new Ui::MainView),
	_clock(this)
{
	ui->setupUi(this);
	ui->_videoView->setClock(&_clock);

	QMenu *menuFile = ui->menubar->addMenu("File");
	QAction *open = new QAction("&Open", this);
	menuFile->addAction(open);
	connect(open, SIGNAL(triggered()), this, SLOT(onOpenFile()));

}


MainView::~MainView()
{
	delete ui;
}

bool MainView::openFile(QString fileName)
{
    QFileInfo fileInfo(fileName);
    if (fileInfo.exists())
    {
		ui->_videoView->open(fileName);
		_clock.setRate(1);
        return true;
    }
	return false;
}

void MainView::onOpenFile()
{
	 QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
	 openFile(fileName); // TODO: show error in case of error
}
