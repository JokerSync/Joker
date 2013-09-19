#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "GoTo.h"
#include "AboutMenu.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _frame = 5882;
    _tcType = PhTimeCodeType25;
    _goto = new GoTo(this);
    _about = new AboutMenu(this);
    ui->timeCodeLabel->setText(PhTimeCode::stringFromFrame(_frame, _tcType));

    connect(_goto, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(updateText(PhFrame, PhTimeCodeType)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateText(PhFrame frame, PhTimeCodeType tcType)
{
    ui->timeCodeLabel->setText(PhTimeCode::stringFromFrame(frame, tcType));
}

void MainWindow::on_actionGoto_triggered()
{
    _goto->show();
}

void MainWindow::on_actionAbout_triggered()
{
    _about->show();
}
