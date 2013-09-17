#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _frame = 5882;
    _tcType = PhTimeCodeType25;
    ui->timeCodeLabel->setText(PhTimeCode::stringFromFrame(_frame, _tcType));

    //connect(ui->timeCodeInput, SIGNAL(frameChanged(PhFrame)), this, SLOT(updateText(PhFrame)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateText(PhFrame frame)
{
    ui->timeCodeLabel->setText(PhTimeCode::stringFromFrame(frame, _tcType));
}
