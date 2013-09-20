#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDlg.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	_stripView = ui->stripView;
	_doc = _stripView->doc();
	_clock = _stripView->clock();

	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFile()));

	connect(_clock, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame, PhTimeCodeType)));
	connect(_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));
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
			_clock->setTimeCodeType(_doc->getTCType());
			_clock->setFrame(_doc->getLastFrame());
			this->setWindowTitle(fileName);
		}
	}
}

void MainWindow::onOpenFile()
{
	QFileDialog dlg(this, "Open...", "", "Rythmo files (*.detx)");
	if(dlg.exec())
	{
		QString fileName = dlg.selectedFiles()[0];
		openFile(fileName);
	}
}

void MainWindow::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromFrame(frame, tcType), QString::number(_clock->rate()));
	ui->statusbar->showMessage(message);
}

void MainWindow::onRateChanged(PhRate rate)
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromFrame(_clock->frame(), _clock->timeCodeType()), QString::number(rate));
	ui->statusbar->showMessage(message);
}

void MainWindow::on_actionPlay_pause_triggered()
{
	if(_clock->rate() == 0.0)
		_clock->setRate(1.0);
	else
		_clock->setRate(0.0);
}

void MainWindow::on_actionPlay_backward_triggered()
{
    _clock->setRate(-1.0);
}

void MainWindow::on_actionStep_forward_triggered()
{
    _clock->setRate(0.0);
	_clock->setFrame(_clock->frame() + 1);
}

void MainWindow::on_actionStep_backward_triggered()
{
	_clock->setRate(0.0);
	_clock->setFrame(_clock->frame() - 1);
}

void MainWindow::on_actionStep_time_forward_triggered()
{
	_clock->setRate(0.0);
	_clock->setTime(_clock->time() + 1);
}

void MainWindow::on_actionStep_time_backward_triggered()
{
	_clock->setRate(0.0);
	_clock->setTime(_clock->time() - 1);
}

void MainWindow::on_action_3_triggered()
{
	_clock->setRate(-3.0);
}

void MainWindow::on_action_1_triggered()
{
	_clock->setRate(-1.0);
}

void MainWindow::on_action_0_5_triggered()
{
	_clock->setRate(-0.5);
}

void MainWindow::on_action0_triggered()
{
	_clock->setRate(0.0);
}

void MainWindow::on_action0_5_triggered()
{
	_clock->setRate(0.5);
}

void MainWindow::on_action1_triggered()
{
	_clock->setRate(1.0);
}

void MainWindow::on_action3_triggered()
{
	_clock->setRate(3.0);
}

void MainWindow::on_actionGo_to_triggered()
{
	PhTimeCodeDlg dlg(_clock->timeCodeType(), _clock->frame());
	if(dlg.exec() == QDialog::Accepted)
		_clock->setFrame(dlg.frame());
}

void MainWindow::on_actionPrevious_Element_triggered()
{
	_clock->setFrame(_doc->getPreviousElementFrame(_clock->frame()));
}

void MainWindow::on_actionNext_Element_triggered()
{
	_clock->setFrame(_doc->getNextElementFrame(_clock->frame()));
}
