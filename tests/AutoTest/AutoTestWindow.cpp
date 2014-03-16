#include "AutoTestWindow.h"
#include "ui_AutoTestWindow.h"

AutoTestWindow::AutoTestWindow(PhWindowSettings *settings) :
	PhWindow(settings),
	ui(new Ui::AutoTestWindow)
{
	ui->setupUi(this);

	connect(ui->actionFull_screen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));
}

AutoTestWindow::~AutoTestWindow()
{
	delete ui;
}

QAction *AutoTestWindow::fullScreenAction()
{
	return ui->actionFull_screen;
}
