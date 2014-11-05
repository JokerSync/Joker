/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QFile>

#include "PhTools/PhDebug.h"

#include "DocumentWindowSpecWindow.h"
#include "ui_DocumentWindowSpecWindow.h"

DocumentWindowSpecWindow::DocumentWindowSpecWindow(DocumentWindowSpecSettings *settings) :
	PhDocumentWindow(settings),
	ui(new Ui::DocumentWindowSpecWindow)
{
	ui->setupUi(this);
}

DocumentWindowSpecWindow::~DocumentWindowSpecWindow()
{
	delete ui;
}

bool DocumentWindowSpecWindow::openDocument(const QString &fileName)
{
	PHDEBUG << fileName;
	QFile file(fileName);
	if(!file.open(QFile::ReadOnly))
		return false;

	QTextStream ts(&file);
	ui->plainTextEdit->setPlainText(ts.readAll());

	return PhDocumentWindow::openDocument(fileName);
}

QMenu *DocumentWindowSpecWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

QString DocumentWindowSpecWindow::text()
{
	return ui->plainTextEdit->toPlainText();
}
