/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "WindowSpecWindow.h"
#include "ui_WindowSpecWindow.h"

WindowSpecWindow::WindowSpecWindow(PhWindowSettings *settings) :
	PhWindow(settings),
	ui(new Ui::WindowSpecWindow)
{
	ui->setupUi(this);

	this->restoreGeometry(settings->windowGeometry());

	connect(ui->actionFull_screen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));
}

WindowSpecWindow::~WindowSpecWindow()
{
	delete ui;
}

QAction *WindowSpecWindow::fullScreenAction()
{
	return ui->actionFull_screen;
}
