#include "GraphicStripTestWindow.h"
#include "ui_GraphicStripTestWindow.h"

#include <QFileDialog>
#include <QFontDialog>
#include <QFont>
#include <QStandardPaths>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"

GraphicStripTestWindow::GraphicStripTestWindow(GraphicStripTestSettings * settings) :
	PhEditableDocumentWindow(settings),
	ui(new Ui::GraphicStripTestWindow),
	_settings(settings),
	_strip(settings)
{
	ui->setupUi(this);

	ui->stripView->setGraphicSettings(settings);

	_doc = _strip.doc();
	_clock = _strip.clock();
	ui->actionInvert_colors->setChecked(_settings->invertColor());

	on_actionRuler_triggered(_settings->displayFeet());

	connect(ui->actionFull_screen, &QAction::triggered, this, &GraphicStripTestWindow::toggleFullScreen);

	connect(_clock, &PhClock::timeChanged, this, &GraphicStripTestWindow::onTimeChanged);
	connect(_clock, &PhClock::rateChanged, this, &GraphicStripTestWindow::onRateChanged);
	this->onTimeChanged(_clock->time());

	if(_settings->generate())
		_doc->generate(_settings->textContent(),
		               _settings->loopNumber(),
		               _settings->peopleNumber(),
		               _settings->spaceBetweenText(),
		               _settings->textNumber(),
		               _settings->trackNumber(),
		               _settings->startTime());

	connect(ui->stripView, &PhGraphicView::beforePaint, _clock, &PhClock::elapse);
	connect(ui->stripView, &PhGraphicView::paint, this, &GraphicStripTestWindow::onPaint);
}

GraphicStripTestWindow::~GraphicStripTestWindow()
{
	delete ui;
}

void GraphicStripTestWindow::on_actionNew_triggered()
{
	_doc->reset();
	this->resetDocument();
}

bool GraphicStripTestWindow::openDocument(const QString &fileName)
{
	PHDEBUG << fileName;
	if(!_doc->openStripFile(fileName))
		return false;

	_settings->setGenerate(false);
	return PhDocumentWindow::openDocument(fileName);
}


void GraphicStripTestWindow::on_actionSave_triggered()
{
	QString fileName = _settings->currentDocument();
	QFileInfo info(fileName);
	if(!info.exists() || (info.suffix() != "detx"))
		on_actionSave_as_triggered();
	else if(_doc->exportDetXFile(fileName, _strip.clock()->time()))
		_doc->setModified(false);
	else
		QMessageBox::critical(this, "", tr("Unable to save ") + fileName);
}

void GraphicStripTestWindow::on_actionSave_as_triggered()
{
	QString fileName = _settings->currentDocument();
	QString lastFolder = _settings->lastDocumentFolder();
	// If there is no current strip file, ask for a name
	if(fileName == "")
		fileName = lastFolder;
	else {
		QFileInfo info(fileName);
		if(info.suffix() != "detx")
			fileName = lastFolder + "/" + info.completeBaseName() + ".detx";
	}

	fileName = QFileDialog::getSaveFileName(this, tr("Save..."), fileName, "*.detx");
	if(fileName != "") {
		if(_doc->exportDetXFile(fileName, _strip.clock()->time())) {
			_doc->setModified(false);
			PhEditableDocumentWindow::saveDocument(fileName);
		}
		else
			QMessageBox::critical(this, "", tr("Unable to save ") + fileName);
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

bool GraphicStripTestWindow::isDocumentModified()
{
	return _doc->modified();
}

void GraphicStripTestWindow::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open...", _settings->lastDocumentFolder(), "Rythmo files (*.joker *.detx)");
	if(QFile::exists(fileName)) {
		if(!openDocument(fileName))
			QMessageBox::critical(this, "Error", "Unable to open " + fileName);
	}
}

void GraphicStripTestWindow::on_actionGenerate_triggered()
{
	GenerateDialog dlgGen(_settings, _doc);
	if (dlgGen.exec()) {

		if(dlgGen.getCheckBoxState()) {
			_clock->setTime(0);
			_settings->setGenerate(true);
			_doc->reset();
			_doc->addPeople(new PhPeople("A people"));
			_doc->addPeople(new PhPeople("A second people", "red"));

			_doc->addText(new PhStripText(0, _doc->peoples().first(), 10000, 1, "Hello", 0.25f));
			_doc->addCut(new PhStripCut(5400, PhStripCut::CrossFade));
			_doc->addDetect(new PhStripDetect(PhStripDetect::Off, 0, _doc->peoples().first(), 10000, 1));
			_doc->addLoop(new PhStripLoop(22000, "3"));
			_doc->addText(new PhStripText(10000, _doc->peoples().last(), 15000, 2, "Hi !", 0.25f));
			_doc->addDetect(new PhStripDetect(PhStripDetect::SemiOff, 10000, _doc->peoples().last(), 15000, 2));
		}
		else {
			_clock->setTime(_doc->lastTime());
			_settings->setGenerate(true);
			openDocument("");
		}
	}
}

void GraphicStripTestWindow::onTimeChanged(PhTime)
{
	QString message = QString("%1 - x%2").arg(_clock->timeCode(_doc->videoTimeCodeType()), QString::number(_clock->rate()));
	ui->statusbar->showMessage(message);
}

void GraphicStripTestWindow::onRateChanged(PhRate rate)
{
	QString message = QString("%1 - x%2").arg(PhTimeCode::stringFromTime(_clock->time(), _doc->videoTimeCodeType()), QString::number(rate));
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
	_clock->setTime(_clock->time() + PhTimeCode::timePerFrame(_doc->videoTimeCodeType()));
}

void GraphicStripTestWindow::on_actionStep_backward_triggered()
{
	_clock->setRate(0.0);
	_clock->setTime(_clock->time() - PhTimeCode::timePerFrame(_doc->videoTimeCodeType()));
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
	PhTimeCodeDialog dlg(_doc->videoTimeCodeType(), _clock->time());
	if(dlg.exec() == QDialog::Accepted)
		_clock->setTime(dlg.time());
}

void GraphicStripTestWindow::on_actionPrevious_Element_triggered()
{
	PhTime time = _doc->previousElementTime(_clock->time());
	if(time > PHTIMEMIN)
		_clock->setTime(time);
}

void GraphicStripTestWindow::on_actionNext_Element_triggered()
{
	PhTime time = _doc->nextElementTime(_clock->time());
	if(time < PHTIMEMAX)
		_clock->setTime(time);
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
	_settings->setDisplayFeet(checked);
	if(checked && _settings->firstFootTime() == 0)
		on_actionChange_ruler_timestamp_triggered();
}

void GraphicStripTestWindow::on_actionChange_ruler_timestamp_triggered()
{
	PhTimeCodeType tcType = _doc->videoTimeCodeType();
	PhTimeCodeDialog dlg(tcType, _settings->firstFootTime(), this);
	if(dlg.exec())
		_settings->setFirstFootTime(dlg.frame() * PhTimeCode::timePerFrame(tcType));
}

void GraphicStripTestWindow::onPaint(int width, int height)
{
	int h = height;
	if(_settings)
		h = height * _settings->stripHeight();
	_strip.draw(0, height - h, width, h);
	foreach(QString info, _strip.infos()) {
		ui->stripView->addInfo(info);
	}
}

void GraphicStripTestWindow::on_actionChange_font_triggered()
{
	QString fontFile = QFileDialog::getOpenFileName(this, "Change font...", "", "Font files (*.ttf)");
	if(QFile(fontFile).exists()) {
		if(PhFont::computeMaxFontSize(fontFile, _settings->textBoldness()) == 0)
			QMessageBox::critical(this, "Error", "Unable to open " + fontFile);
		else
			_settings->setTextFontFile(fontFile);
	}
}
