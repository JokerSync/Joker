#include "GraphicStripTestWindow.h"
#include "ui_GraphicStripTestWindow.h"

#include <QFileDialog>
#include <QFontDialog>
#include <QFont>
#include <QStandardPaths>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"

GraphicStripTestWindow::GraphicStripTestWindow(GraphicStripTestSettings * settings) :
	PhDocumentWindow(settings),
	ui(new Ui::GraphicStripTestWindow),
	_settings(settings)
{
	ui->setupUi(this);
	_strip = ui->stripView->strip();

	ui->stripView->setStripSettings(_settings);

	_doc = _strip->doc();
	_clock = _strip->clock();
	ui->actionInvert_colors->setChecked(_settings->invertColor());
	ui->actionRuler->setChecked(_settings->displayRuler());

	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(onOpenFile()));
	connect(ui->actionGenerate, SIGNAL(triggered()), this, SLOT(onGenerate()));

	connect(ui->actionFull_screen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));

	connect(_clock, SIGNAL(frameChanged(PhFrame, PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame, PhTimeCodeType)));
	connect(_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));
}

GraphicStripTestWindow::~GraphicStripTestWindow()
{
	delete ui;
}

bool GraphicStripTestWindow::openDocument(QString fileName)
{
	PHDEBUG << fileName;
	if(!_doc->openStripFile(fileName))
		return false;

	_clock->setTimeCodeType(_doc->timeCodeType());
	setCurrentDocument(fileName);
	return true;
}

void GraphicStripTestWindow::createFile(int nbPeople, int nbLoop, int nbText, int nbTrack, QString text, int videoTimeStamp)
{
	PHDEBUG << "Creating fake file";
	if(_doc->create(text, nbPeople, nbText, nbTrack, videoTimeStamp)) {
		PHDEBUG << "Done";
		_clock->setTimeCodeType(_doc->timeCodeType());
		_clock->setFrame(_doc->lastFrame());
		this->setWindowTitle("GraphicStripTest");
	}
}

QMenu *GraphicStripTestWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

QAction *GraphicStripTestWindow::fullScreenAction()
{
	return ui->actionFull_screen;
}

void GraphicStripTestWindow::onOpenFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open...", _settings->lastDocumentFolder(), "Rythmo files (*.strip *.detx)");
	if(QFile::exists(fileName)) {
		if(!openDocument(fileName))
			QMessageBox::critical(this, "Error", "Unable to open " + fileName);
	}
}

void GraphicStripTestWindow::onGenerate()
{
	GenerateDialog dlgGen(_settings, _doc);
	if (dlgGen.exec()) {
		_clock->setFrame(_doc->lastFrame());
		setCurrentDocument("");
	}
}

void GraphicStripTestWindow::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromFrame(frame, tcType), QString::number(_clock->rate()));
	ui->statusbar->showMessage(message);
}

void GraphicStripTestWindow::onRateChanged(PhRate rate)
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromFrame(_clock->frame(), _clock->timeCodeType()), QString::number(rate));
	ui->statusbar->showMessage(message);
}

void GraphicStripTestWindow::on_actionPlay_pause_triggered()
{
	if(_clock->rate() == 0.0)
		_clock->setRate(1.0);
	else
		_clock->setRate(0.0);
}

void GraphicStripTestWindow::on_actionPlay_backward_triggered()
{
	_clock->setRate(-1.0);
}

void GraphicStripTestWindow::on_actionStep_forward_triggered()
{
	_clock->setRate(0.0);
	_clock->setFrame(_clock->frame() + 1);
}

void GraphicStripTestWindow::on_actionStep_backward_triggered()
{
	_clock->setRate(0.0);
	_clock->setFrame(_clock->frame() - 1);
}

void GraphicStripTestWindow::on_actionStep_time_forward_triggered()
{
	_clock->setRate(0.0);
	_clock->setTime(_clock->time() + 1);
}

void GraphicStripTestWindow::on_actionStep_time_backward_triggered()
{
	_clock->setRate(0.0);
	_clock->setTime(_clock->time() - 1);
}

void GraphicStripTestWindow::on_action_3_triggered()
{
	_clock->setRate(-3.0);
}

void GraphicStripTestWindow::on_action_1_triggered()
{
	_clock->setRate(-1.0);
}

void GraphicStripTestWindow::on_action_0_5_triggered()
{
	_clock->setRate(-0.5);
}

void GraphicStripTestWindow::on_action0_triggered()
{
	_clock->setRate(0.0);
}

void GraphicStripTestWindow::on_action0_5_triggered()
{
	_clock->setRate(0.5);
}

void GraphicStripTestWindow::on_action1_triggered()
{
	_clock->setRate(1.0);
}

void GraphicStripTestWindow::on_action3_triggered()
{
	_clock->setRate(3.0);
}

void GraphicStripTestWindow::on_actionGo_to_triggered()
{
	PhTimeCodeDialog dlg(_clock->timeCodeType(), _clock->frame());
	if(dlg.exec() == QDialog::Accepted)
		_clock->setFrame(dlg.frame());
}

void GraphicStripTestWindow::on_actionPrevious_Element_triggered()
{
	_clock->setFrame(_doc->previousElementFrame(_clock->frame()));
}

void GraphicStripTestWindow::on_actionNext_Element_triggered()
{
	_clock->setFrame(_doc->nextElementFrame(_clock->frame()));
}

void GraphicStripTestWindow::on_actionStrip_Properties_triggered()
{
	dlg = new StripPropertiesDialog(_doc, this);
	dlg->show();
}

void GraphicStripTestWindow::on_actionInvert_colors_triggered(bool checked)
{
	_settings->setInvertColor(checked);
}


void GraphicStripTestWindow::on_actionRuler_triggered(bool checked)
{
	_settings->setDisplayRuler(checked);
	if(checked && _settings->rulerTimestamp() == 0)
		on_actionChange_ruler_timestamp_triggered();
}

void GraphicStripTestWindow::on_actionChange_ruler_timestamp_triggered()
{
	PhTimeCodeDialog dlg(_doc->timeCodeType(), _settings->rulerTimestamp(), this);
	if(dlg.exec())
		_settings->setRulerTimestamp(dlg.frame());
}
