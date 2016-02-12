/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhDebug.h"

#include "WindowSpecWindow.h"
#include "ui_WindowSpecWindow.h"

WindowSpecWindow::WindowSpecWindow(PhDocumentWindowSettings *settings) :
	PhDocumentWindow(settings),
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

bool WindowSpecWindow::openDocument(const QString &fileName)
{
	PHDEBUG << fileName;
	QFile file(fileName);
	if(!file.open(QFile::ReadOnly))
		return false;

	QTextStream ts(&file);
	ui->plainTextEdit->setPlainText(ts.readAll());

	return PhDocumentWindow::openDocument(fileName);
}

QMenu *WindowSpecWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

QString WindowSpecWindow::text()
{
	return ui->plainTextEdit->toPlainText();
}
