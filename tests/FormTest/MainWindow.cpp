#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Dialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _goto = new QDialog(this);
    _frame = 5882;
    _tcType = PhTimeCodeType25;
    ui->timeCodeLabel->setText(PhTimeCode::stringFromFrame(_frame, _tcType));

    connect(_goto, SIGNAL(frameChanged(PhFrame)), this, SLOT(updateText(PhFrame)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateText(PhFrame frame)
{
    ui->timeCodeLabel->setText(PhTimeCode::stringFromFrame(frame, _tcType));
}

void MainWindow::on_actionGoto_triggered()
{
    _goto->show();
}
