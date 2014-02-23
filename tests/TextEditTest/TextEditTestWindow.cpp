#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

#include <PhTools/PhDebug.h>

#include "TextEditTestWindow.h"
#include "ui_TextEditTestWindow.h"

TextEditTestWindow::TextEditTestWindow(TextEditTestSettings *settings, QWidget *parent) :
	PhDocumentWindow(settings, parent),
	ui(new Ui::TextEditTestWindow),
	_settings(settings)
{
	ui->setupUi(this);
}

TextEditTestWindow::~TextEditTestWindow()
{
	delete ui;
}

bool TextEditTestWindow::openFile(QString fileName)
{
	QFile file(fileName);
	if(!file.open(QFile::ReadOnly))
		return false;

	QTextStream ts(&file);
	ui->textEdit->setText(ts.readAll());

	setCurrentDocument(fileName);
	return true;
}

QMenu *TextEditTestWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

void TextEditTestWindow::on_actionOpen_triggered()
{
	PHDEBUG << _settings->lastDocumentFolder();
	QString fileName = QFileDialog::getOpenFileName(this, "Open a text file...", _settings->lastDocumentFolder(), "Text file (*.txt)");

	if(QFile::exists(fileName)) {
		if(!openFile(fileName))
			QMessageBox::critical(this, "", tr("Unable to open ") + fileName);
	}
}

void TextEditTestWindow::on_actionSave_triggered()
{

}

void TextEditTestWindow::on_actionSave_as_triggered()
{

}
