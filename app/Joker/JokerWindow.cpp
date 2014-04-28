/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "JokerWindow.h"
#include "ui_JokerWindow.h"

#include <QFileDialog>
#include <QFontDialog>
#include <QFileOpenEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QWindowStateChangeEvent>
#include <QMouseEvent>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PhCommonUI/PhFeedbackDialog.h"
#include "AboutDialog.h"
#include "PreferencesDialog.h"
#include "PeopleDialog.h"

JokerWindow::JokerWindow(JokerSettings *settings) :
	PhDocumentWindow(settings),
	ui(new Ui::JokerWindow),
	_settings(settings),
	_sonySlave(PhTimeCodeType25, settings),
	_mediaPanelAnimation(&_mediaPanel, "windowOpacity"),
	_needToSave(false),
	_firstDoc(true)
{
	// Setting up UI
	ui->setupUi(this);

	// Due to translation, Qt might not be able to link automatically the menu
	ui->actionPreferences->setMenuRole(QAction::PreferencesRole);
	ui->actionAbout->setMenuRole(QAction::AboutRole);

	connect(ui->actionFullscreen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));

	// Get the pointer to the differents objects :
	// strip, video engine and doc
	_strip = ui->videoStripView->strip();
	_videoEngine = ui->videoStripView->videoEngine();
	_doc = _strip->doc();

	// Pass the settings to the modules
	_strip->setSettings(_settings);
	_videoEngine->setSettings(_settings);
	ui->videoStripView->setSettings(_settings);

	// Initialize the property dialog
	_propertyDialog.setDoc(_doc);
	_propertyDialog.setVideoEngine(_videoEngine);

	// Initialize the synchronizer
	_synchronizer.setStripClock(_strip->clock());
	_synchronizer.setVideoClock(_videoEngine->clock());

	setupSyncProtocol();

	// Setting up the media panel
	_mediaPanel.setClock(_strip->clock());
#warning /// @todo move to CSS file
	_mediaPanel.setStyleSheet(
	    "* {"
	    "	  color: white;"
	    "  }"
	    "  PhMediaPanel { "
	    "	  background: qlineargradient(x1: 1, y1: 0, x2: 1, y2: 1, stop: 0 rgb(40,40,40), stop: 1 black);"
	    "	  border-style: solid;                                                                          "
	    "	  border-width: 4px;                                                                            "
	    "	  border-radius: 3px;                                                                           "
	    "	  border-color: white;                                                                          "
	    "  }                                                                                                "
	    "  QPushButton, QComboBox{                                                                          "
	    "	  background: grey;                                                                             "
	    "	  border-style: outset;                                                                         "
	    "	  border-width: 2px;                                                                            "
	    "	  border-radius: 5px;                                                                           "
	    "	  border-color: white;                                                                          "
	    "  }                                                                                                "
	    "  QLabel#_timecodeLabel{                                                                           "
	    "	  padding: 10px;                                                                                "
	    "  }                                                                                                "
	    );

	this->setFocus();

	if(_settings->stripTestMode()) {
#warning /// @todo do we warn the user that test mode is on?
		ui->actionTest_mode->setChecked(true);
	}

#warning /// @todo move to PhDocumentWindow
	// This is for the drag and drop feature
	setAcceptDrops(true);

	ui->actionInvert_colors->setChecked(_settings->invertColor());

	ui->actionShow_ruler->setChecked(_settings->displayRuler());

	if(!_settings->exitedNormaly())
		on_actionSend_feedback_triggered();

	_settings->setExitedNormaly(false);

	_mediaPanel.show();
	_mediaPanelState = MediaPanelVisible;

	// Trigger a timer that will fade off the media panel after 3 seconds
	this->connect(&_mediaPanelTimer, SIGNAL(timeout()), this, SLOT(fadeOutMediaPanel()));
	_mediaPanelTimer.start(3000);
}

JokerWindow::~JokerWindow()
{
	delete ui;
}

void JokerWindow::setupSyncProtocol()
{
	PhClock* clock = NULL;

	// Disable old protocol
	_sonySlave.close();
	_ltcReader.close();

	Synchronizer::SyncType type = (Synchronizer::SyncType)_settings->synchroProtocol();

	switch(type) {
	case Synchronizer::Sony:
		// Initialize the sony module
		if(_sonySlave.open()) {
			clock = _sonySlave.clock();
			ui->videoStripView->setSony(&_sonySlave);
		}
		else {
			//type = Synchronizer::NoSync;
			QMessageBox::critical(this, "", "Unable to connect to USB422v module");
		}
		break;
	case Synchronizer::LTC:
		{
			QString input = _settings->ltcInputDevice();
			if(_ltcReader.init(input))
				clock = _ltcReader.clock();
			else {
				QMessageBox::critical(this, "", "Unable to open " + input);
				//type = Synchronizer::NoSync;
			}
			break;
		}
	case Synchronizer::NoSync:
		break;
	}

	_synchronizer.setSyncClock(clock, type);

	// Disable slide if Joker is sync to a protocol
	_mediaPanel.setSliderEnable(clock == NULL);

	_settings->setSynchroProtocol(type);
}

bool JokerWindow::openDocument(QString fileName)
{
	/// Clear the selected people name list (except for the first document).
	if(!_firstDoc)
		_settings->setSelectedPeopleNameList(QStringList());
	else
		_firstDoc = false;

	if(!_doc->openStripFile(fileName))
		return false;

	/// If the document is opened successfully :
	/// - Update the current document name (settings, windows title)
	setCurrentDocument(fileName);
	_watcher.addPath(_doc->filePath());

	/// - Open the corresponding video file if it exists.
	if(openVideoFile(_doc->videoFilePath())) {
		PhFrame frameIn = _doc->videoFrameIn();
		_videoEngine->setFirstFrame(frameIn);
		_videoEngine->setDeinterlace(_doc->videoDeinterlace());
		ui->actionDeinterlace_video->setChecked(_doc->videoDeinterlace());
		_mediaPanel.setFirstFrame(frameIn);
	}
	else
		_videoEngine->close();


	/// - Set the video aspect ratio.
	ui->actionForce_16_9_ratio->setChecked(_doc->forceRatio169());
	ui->videoStripView->setForceRatio169(_doc->forceRatio169());

	/// - Use the document timecode type.
	_strip->clock()->setTimeCodeType(_doc->timeCodeType());
	/// - Goto to the document last position.
	_strip->clock()->setTime(_doc->lastTime());
	/// - Disable the need to save flag.
	_needToSave = false;

	return true;
}

bool JokerWindow::eventFilter(QObject * sender, QEvent *event)
{
	/// The event filter catch the following event:
	switch (event->type()) {
	case QEvent::FileOpen: /// - FileOpen : To process a file dragged on the application dock icon (MacOS)
		{
#warning /// @todo move to PhDocumentWindow
			QString filePath = static_cast<QFileOpenEvent *>(event)->file();
			QString fileType = filePath.split(".").last().toLower();
			// As the plist file list all the supported format (which are .joker, .strip, .detx, .avi & .mov)
			// if the file is not a strip or a detx file, it's a video file, we don't need any protection
			if(fileType == "detx" or fileType == "strip" or fileType == "joker") {
				if(checkSaveFile())
					openDocument(filePath);
			}
			else
				openVideoFile(filePath);
			break;
		}
	case QEvent::ApplicationDeactivate: /// - ApplicationDeactivate : to hide the mediapanel
		hideMediaPanel();
		break;
	case QEvent::MouseMove: /// - Mouse move show the media panel
		fadeInMediaPanel();
		break;
	case QEvent::DragEnter: /// - Accept and process a file drop on the window
		event->accept();
		break;
	case QEvent::Drop:
		{
#warning /// @todo move to PhDocumentWindow
			const QMimeData* mimeData = static_cast<QDropEvent *>(event)->mimeData();

			// If there is one file (not more) we open it
			if (mimeData->urls().length() == 1) {
				QString filePath = mimeData->urls().first().toLocalFile();
				QString fileType = filePath.split(".").last().toLower();
				if(fileType == "detx" or fileType == "strip" or fileType == "joker") {
					if(checkSaveFile())
						openDocument(filePath);
				}
				else if (fileType == "avi" or fileType == "mov")
					openVideoFile(filePath);
			}
			break;
		}
	case QEvent::MouseButtonDblClick: /// - Double mouse click toggle fullscreen mode
		if(sender == this)
			toggleFullScreen();
		break;
	case QEvent::MouseButtonPress:
		{
			QMouseEvent *mouseEvent = (QMouseEvent*)event;
			//PHDEBUG << sender << mouseEvent->buttons() << mouseEvent->pos() << this->pos();
			if((sender == this) && (mouseEvent->buttons() & Qt::RightButton)) {
				/// - Right mouse click on the video open the video file dialog.
				if(mouseEvent->y() < this->height() * (1.0f - _settings->stripHeight()))
					on_actionOpen_Video_triggered();
				else /// - Left mouse click on the strip open the strip file dialog.
					on_actionOpen_triggered();
				return true;
			}
		}
	default:
		break;
	}

	return PhDocumentWindow::eventFilter(sender, event);
}

QMenu *JokerWindow::recentDocumentMenu()
{
	return ui->menuOpen_recent;
}

QAction *JokerWindow::fullScreenAction()
{
	return ui->actionFullscreen;
}

void JokerWindow::closeEvent(QCloseEvent *event)
{
	/// Check if the current document has to be saved (it might cancel the action).
	if(!checkSaveFile())
		event->ignore();
	else /// Close the PhMediaPanel.
		_mediaPanel.close();
}

void JokerWindow::on_actionOpen_triggered()
{
	hideMediaPanel();

	if(checkSaveFile()) {
		QString filter = tr("Rythmo files") + " (*.joker *.detx *.mos *.strip);; "
		                 + tr("Joker files") + " (*.joker);; "
		                 + tr("DetX files") + " (*.detx);; "
		                 + tr("Mosaic files") + " (*.mos);; "
		                 + tr("All files") + " (*.*)";
		QFileDialog dlg(this, tr("Open..."), _settings->lastDocumentFolder(), filter);

		dlg.selectNameFilter(_settings->selectedFilter());
		dlg.setOption(QFileDialog::HideNameFilterDetails, false);

		dlg.setFileMode(QFileDialog::ExistingFile);
		if(dlg.exec()) {
			QString fileName = dlg.selectedFiles()[0];
			openDocument(fileName);
			_settings->setSelectedFilter(dlg.selectedNameFilter());
		}
	}
	fadeInMediaPanel();
}

void JokerWindow::on_actionPlay_pause_triggered()
{
	if(_strip->clock()->rate() == 0.0)
		_strip->clock()->setRate(1.0);
	else
		_strip->clock()->setRate(0.0);
}

void JokerWindow::on_actionPlay_backward_triggered()
{
	_strip->clock()->setRate(-1.0);
}

void JokerWindow::on_actionStep_forward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setFrame(_strip->clock()->frame() + 1);
}

void JokerWindow::on_actionStep_backward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setFrame(_strip->clock()->frame() - 1);
}

void JokerWindow::on_actionStep_time_forward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setTime(_strip->clock()->time() + 1);
}

void JokerWindow::on_actionStep_time_backward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setTime(_strip->clock()->time() - 1);
}

void JokerWindow::on_action_3_triggered()
{
	_strip->clock()->setRate(-3.0);
}

void JokerWindow::on_action_1_triggered()
{
	_strip->clock()->setRate(-1.0);
}

void JokerWindow::on_action_0_5_triggered()
{
	_strip->clock()->setRate(-0.5);
}

void JokerWindow::on_action0_triggered()
{
	_strip->clock()->setRate(0.0);
}

void JokerWindow::on_action0_5_triggered()
{
	_strip->clock()->setRate(0.5);
}

void JokerWindow::on_action1_triggered()
{
	_strip->clock()->setRate(1.0);
}

void JokerWindow::on_action3_triggered()
{
	_strip->clock()->setRate(3.0);
}

void JokerWindow::on_actionOpen_Video_triggered()
{
	hideMediaPanel();

	QString lastFolder = _settings->lastVideoFolder();
	QString filter = tr("Movie files") + _settings->videoFileFilter()
	                 + ";;" + tr("All files") + " (*.*)";

	QFileDialog dlg(this, tr("Open a video..."), lastFolder, filter);
	if(dlg.exec()) {
		QString videoFile = dlg.selectedFiles()[0];
		if(openVideoFile(videoFile))
			_strip->clock()->setTime(_doc->videoTimeIn());
	}

	fadeInMediaPanel();
}

bool JokerWindow::openVideoFile(QString videoFile)
{
	QFileInfo lastFileInfo(_doc->videoFilePath());
	QFileInfo fileInfo(videoFile);
	if (fileInfo.exists() && _videoEngine->open(videoFile)) {
		PhFrame frameIn = _videoEngine->firstFrame();

		_mediaPanel.setFirstFrame(frameIn);
		_mediaPanel.setMediaLength(_videoEngine->length());

		if(videoFile != _doc->videoFilePath()) {
			_doc->setVideoFilePath(videoFile);
			if(frameIn > 0)
				_doc->setVideoFrameIn(frameIn);
			_needToSave = true;
		}

		if(frameIn == 0) {
			frameIn = _doc->videoFrameIn();
			_videoEngine->setFirstFrame(frameIn);
			_videoEngine->clock()->setFrame(frameIn);
			if(fileInfo.fileName() != lastFileInfo.fileName()) {
				on_actionChange_timestamp_triggered();
				frameIn = _videoEngine->firstFrame();
			}
		}

		_videoEngine->clock()->setFrame(frameIn);

		_settings->setLastVideoFolder(fileInfo.absolutePath());
		return true;
	}
	return false;
}

void JokerWindow::on_actionChange_timestamp_triggered()
{
	hideMediaPanel();
	PhFrame frame;
	if(_synchronizer.videoClock()->frame() < _videoEngine->firstFrame())
		frame = _videoEngine->firstFrame();
	else if(_synchronizer.videoClock()->frame() > _videoEngine->firstFrame() + _videoEngine->length())
		frame = _videoEngine->lastFrame();
	else
		frame = _synchronizer.videoClock()->frame();

	PhTimeCodeDialog dlg(_strip->clock()->timeCodeType(), frame);
	if(dlg.exec() == QDialog::Accepted) {
		PhFrame frameStamp;
		if(_synchronizer.videoClock()->frame() > _videoEngine->firstFrame() + _videoEngine->length())
			frameStamp = dlg.frame() - (_videoEngine->length() - 1);
		else if (_synchronizer.videoClock()->frame() < _videoEngine->firstFrame())
			frameStamp =  dlg.frame();
		else
			frameStamp = _videoEngine->firstFrame() + dlg.frame() - _synchronizer.videoClock()->frame();

		_videoEngine->setFirstFrame(frameStamp);
		_strip->clock()->setFrame(dlg.frame());
		_doc->setVideoFrameIn(frameStamp);
		_mediaPanel.setFirstFrame(frameStamp);
		_needToSave = true;
	}

	fadeInMediaPanel();
}



void JokerWindow::on_actionAbout_triggered()
{
	hideMediaPanel();

	AboutDialog menu;
	menu.exec();

	fadeInMediaPanel();
}


void JokerWindow::on_actionPreferences_triggered()
{
	hideMediaPanel();
	int oldSynchroProtocol = _settings->synchroProtocol();
	QString oldLTCInputDevice = _settings->ltcInputDevice();

	PreferencesDialog dlg(_settings);
	dlg.exec();
	if((oldSynchroProtocol != _settings->synchroProtocol()) || (oldLTCInputDevice != _settings->ltcInputDevice())) {
		PHDEBUG << "Set protocol:" << _settings->synchroProtocol();
		setupSyncProtocol();
	}

	fadeInMediaPanel();
}

void JokerWindow::fadeInMediaPanel()
{
	// Don't show the mediaPanel if Joker has not thefocus.
	if(!this->hasFocus())
		return;
	// Don't show the mediaPanel if Joker is remote controled.
	if(_settings->synchroProtocol() != Synchronizer::NoSync)
		return;

	_mediaPanel.show();
	_mediaPanelAnimation.stop();
	_mediaPanelAnimation.setDuration(300);
	_mediaPanelAnimation.setEndValue(1);
	_mediaPanelAnimation.setEasingCurve(QEasingCurve::InOutSine);
	_mediaPanelAnimation.start();
	_mediaPanelState = MediaPanelVisible;
	_mediaPanelTimer.start(3000);
}

void JokerWindow::fadeOutMediaPanel()
{
	// Don't fade out the media panel if the mouse is over it
	if(_mediaPanel.underMouse() or _mediaPanel.isMousePressed()) {
		PHDEBUG << "Don't hide";
		_mediaPanelTimer.start(3000);
		return;
	}

	PHDEBUG << _mediaPanelState;
	switch(_mediaPanelState) {
	case MediaPanelVisible:
		PHDEBUG << "Hiding";
		_mediaPanelAnimation.setDuration(1000);
		_mediaPanelAnimation.setEndValue(0);
		_mediaPanelAnimation.setEasingCurve(QEasingCurve::InOutSine);
		_mediaPanelAnimation.start();
		_mediaPanelTimer.start(1000);
		_mediaPanelState = MediaPanelHidding;
		break;
	case MediaPanelHidding:
		hideMediaPanel();
		break;
	default:
		break;
	}
}

void JokerWindow::hideMediaPanel()
{
	_mediaPanel.hide();
	_mediaPanelState = MediaPanelHidden;
	_mediaPanelTimer.stop();
}


void JokerWindow::on_actionProperties_triggered()
{
	_propertyDialog.show();
}

void JokerWindow::on_actionTest_mode_triggered()
{
	_settings->setStripTestMode(!_settings->stripTestMode());
}

void JokerWindow::on_actionTimecode_triggered()
{
	hideMediaPanel();

	PhTimeCodeDialog dlg(_strip->clock()->timeCodeType(), _strip->clock()->frame());
	if(dlg.exec() == QDialog::Accepted)
		_strip->clock()->setFrame(dlg.frame());

	fadeInMediaPanel();
}

void JokerWindow::on_actionNext_element_triggered()
{
	PhTime time = _doc->nextElementTime(_strip->clock()->time());
	if(time < PHTIMEMAX)
		_strip->clock()->setTime(time);
}

void JokerWindow::on_actionPrevious_element_triggered()
{
	PhTime time = _doc->previousElementTime(_strip->clock()->time());
	if(time > PHTIMEMIN)
		_strip->clock()->setTime(time);
}

void JokerWindow::on_actionClear_list_triggered()
{
	//Open the recent group
	//	_settings->beginGroup("openRecent");
	//	//List all keys
	//	QStringList indexes = _settings->allKeys();
	//	//Remove them from
	//	foreach(QString index, indexes)
	//	_settings->remove(index);

	//	//Close the group
	//	_settings->endGroup();

	//Remove the buttons of the UI, keep the separator and the Clear button
	foreach(QAction * action, ui->menuOpen_recent->actions())
	{
		// Break if the separator is reached
		if(action->isSeparator())
			break;
		// Remove it
		ui->menuOpen_recent->removeAction(action);
		delete action;
	}

	// Remove all the buttons
	//	_recentFileButtons.clear();
	ui->menuOpen_recent->setEnabled(false);
}

void JokerWindow::on_actionSave_triggered()
{
	QString fileName = _settings->currentDocument();
	QFileInfo info(fileName);
	if(!info.exists() || (info.suffix() != "joker"))
		on_actionSave_as_triggered();
	else if(_doc->saveStripFile(fileName, _strip->clock()->timeCode()))
		_needToSave = false;
	else
		QMessageBox::critical(this, "", tr("Unable to save ") + fileName);
}

void JokerWindow::on_actionSave_as_triggered()
{
	hideMediaPanel();

	QString fileName = _settings->currentDocument();
	QString lastFolder = _settings->lastDocumentFolder();
	// If there is no current strip file, ask for a name
	if(fileName == "")
		fileName = lastFolder;
	else {
		QFileInfo info(fileName);
		if(info.suffix() != "joker")
			fileName = lastFolder + "/" + info.completeBaseName() + ".joker";
	}

	fileName = QFileDialog::getSaveFileName(this, tr("Save..."), fileName, "*.joker");
	if(fileName != "") {
		if(_doc->saveStripFile(fileName, _strip->clock()->timeCode())) {
			_needToSave = false;
			setCurrentDocument(fileName);
		}
		else
			QMessageBox::critical(this, "", tr("Unable to save ") + fileName);
	}
}

bool JokerWindow::checkSaveFile()
{

	if(_needToSave) {
		/// If the document need to be saved, ask the user
		/// whether he wants to save his changes.
		QString msg = tr("Do you want to save your changes ?");
		QMessageBox box(QMessageBox::Question, "", msg, QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
		box.setDefaultButton(QMessageBox::Save);
		switch(box.exec()) {
		/// Cancel the caller action if clicking cancel.
		case QMessageBox::Cancel:
			return false;
		/// Trigger the document save if clicking save:
		case QMessageBox::Save:
			on_actionSave_triggered();
			/// If the user cancel the save operation, cancel the operation.
			if(_needToSave)
				return false;
			break;
		}
	}
	/// @return False to interrupt the caller action, true otherwhise.
	return true;
}

void JokerWindow::on_actionSelect_character_triggered()
{
	hideMediaPanel();

	PeopleDialog dlg(this, _doc, _settings);

	dlg.restoreGeometry(_settings->peopleDialogGeometry());
	dlg.exec();
	_settings->setPeopleDialogGeometry(dlg.saveGeometry());
}

void JokerWindow::on_actionForce_16_9_ratio_triggered()
{
	ui->videoStripView->setForceRatio169(ui->actionForce_16_9_ratio->isChecked());
	_needToSave = true;
}

void JokerWindow::on_actionInvert_colors_toggled(bool checked)
{
	_settings->setInvertColor(checked);
}

void JokerWindow::on_actionShow_ruler_toggled(bool display)
{
	_settings->setDisplayRuler(display);
}

void JokerWindow::on_actionChange_ruler_timestamp_triggered()
{
	PhTimeCodeType tcType = _doc->timeCodeType();
	PhTimeCodeDialog dlg(tcType, _settings->rulerTimeIn() / PhTimeCode::timePerFrame(tcType), this);
	if(dlg.exec())
		_settings->setRulerTimeIn(dlg.frame() * PhTimeCode::timePerFrame(tcType));
}

void JokerWindow::on_actionNew_triggered()
{
	_doc->reset();
	on_actionClose_video_triggered();
}

void JokerWindow::on_actionClose_video_triggered()
{
	_videoEngine->close();
}

void JokerWindow::on_actionSend_feedback_triggered()
{
	hideMediaPanel();
	PhFeedbackDialog dlg(_settings, this);
	dlg.exec();
	fadeInMediaPanel();
}

void JokerWindow::on_actionDeinterlace_video_triggered(bool checked)
{
	_videoEngine->setDeinterlace(checked);
	if(checked != _doc->videoDeinterlace()) {
		_doc->setVideoDeinterlace(checked);
		_needToSave = true;
	}
}
