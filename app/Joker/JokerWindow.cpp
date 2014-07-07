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
	_strip(settings),
	_videoEngine(settings),
	_doc(_strip.doc()),
	_sonySlave(PhTimeCodeType25, settings),
	_mediaPanelAnimation(&_mediaPanel, "windowOpacity"),
	_firstDoc(true),
	_numberOfDraw(0),
	_resizingStrip(false)
{
	// Setting up UI
	ui->setupUi(this);

	// Due to translation, Qt might not be able to link automatically the menu
	ui->actionPreferences->setMenuRole(QAction::PreferencesRole);
	ui->actionAbout->setMenuRole(QAction::AboutRole);

	connect(ui->actionFullscreen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));

	ui->videoStripView->setGraphicSettings(_settings);

	// Initialize the property dialog
	_propertyDialog.setDoc(_doc);
	_propertyDialog.setVideoEngine(&_videoEngine);

	// Initialize the synchronizer
	_synchronizer.setStripClock(_strip.clock());
	_synchronizer.setVideoClock(_videoEngine.clock());

	connect(&_sonySlave, &PhSonySlaveController::videoSync, this, &JokerWindow::onVideoSync);

	setupSyncProtocol();

	// Setting up the media panel
	_mediaPanel.setClock(_strip.clock());
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

	ui->actionDisplay_the_cuts->setChecked(_settings->displayCuts());

	ui->actionInvert_colors->setChecked(_settings->invertColor());

	ui->actionShow_ruler->setChecked(_settings->displayRuler());

	ui->actionHide_the_rythmo->setChecked(_settings->hideStrip());

	_mediaPanel.show();
	_mediaPanelState = MediaPanelVisible;

	// Trigger a timer that will fade off the media panel after 3 seconds
	this->connect(&_mediaPanelTimer, SIGNAL(timeout()), this, SLOT(fadeOutMediaPanel()));
	_mediaPanelTimer.start(3000);

	this->connect(ui->videoStripView, &PhGraphicView::beforePaint, this, &JokerWindow::timeCounter);
	this->connect(ui->videoStripView, &PhGraphicView::beforePaint, _strip.clock(), &PhClock::tick);

	this->connect(ui->videoStripView, &PhGraphicView::paint, this, &JokerWindow::onPaint);

	_videoLogo.setFilename(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/phonations.png");
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
			_lastVideoSyncElapsed.start();
		}
		else {
			type = Synchronizer::NoSync;
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
				type = Synchronizer::NoSync;
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

	/// - Load the deinterlace settings
	_videoEngine.setDeinterlace(_doc->videoDeinterlace());
	ui->actionDeinterlace_video->setChecked(_doc->videoDeinterlace());

	/// - Open the corresponding video file if it exists.
	if(openVideoFile(_doc->videoFilePath())) {
		PhFrame frameIn = _doc->videoFrameIn();
		_videoEngine.setFirstFrame(frameIn);
		_mediaPanel.setFirstFrame(frameIn);
	}
	else
		_videoEngine.close();


	/// - Set the video aspect ratio.
	ui->actionForce_16_9_ratio->setChecked(_doc->forceRatio169());

	/// - Use the document timecode type.
	_strip.clock()->setTimeCodeType(_doc->timeCodeType());
	/// - Goto to the document last position.
	_strip.clock()->setTime(_doc->lastTime());
	/// - Disable the need to save flag.

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
		{
			fadeInMediaPanel();

			// Check if it is near the video/strip border
			QMouseEvent * mouseEvent = (QMouseEvent*)event;
			if(_resizingStrip) {
				QApplication::setOverrideCursor(Qt::SizeVerCursor);
				if(mouseEvent->buttons() & Qt::LeftButton)
					_settings->setStripHeight(1.0 - ((float) mouseEvent->pos().y() /(float) this->height()));
			}
			else
				QApplication::setOverrideCursor(Qt::ArrowCursor);
			break;
		}
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
		_resizingStrip = false;
		if(sender == this)
			toggleFullScreen();
		break;
	case QEvent::MouseButtonRelease:
		QApplication::setOverrideCursor(Qt::ArrowCursor);
		break;
	case QEvent::MouseButtonPress:
		{
			QMouseEvent *mouseEvent = (QMouseEvent*)event;
			if((sender == this) && (mouseEvent->buttons() & Qt::RightButton)) {
				/// - Right mouse click on the video open the video file dialog.
				if(mouseEvent->y() < this->height() * (1.0f - _settings->stripHeight()))
					on_actionOpen_Video_triggered();
				else /// - Left mouse click on the strip open the strip file dialog.
					on_actionOpen_triggered();
				return true;
			}
			float stripHeight = this->height() * _settings->stripHeight();
			if((mouseEvent->pos().y() > (this->height() - stripHeight) - 10)
			   && (mouseEvent->pos().y() < (this->height() - stripHeight) + 10)) {
				QApplication::setOverrideCursor(Qt::SizeVerCursor);
				_resizingStrip = true;
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
	if(_strip.clock()->rate() == 0.0)
		_strip.clock()->setRate(1.0);
	else
		_strip.clock()->setRate(0.0);
}

void JokerWindow::on_actionPlay_backward_triggered()
{
	_strip.clock()->setRate(-1.0);
}

void JokerWindow::on_actionStep_forward_triggered()
{
	_strip.clock()->setRate(0.0);
	_strip.clock()->setFrame(_strip.clock()->frame() + 1);
}

void JokerWindow::on_actionStep_backward_triggered()
{
	_strip.clock()->setRate(0.0);
	_strip.clock()->setFrame(_strip.clock()->frame() - 1);
}

void JokerWindow::on_actionStep_time_forward_triggered()
{
	_strip.clock()->setRate(0.0);
	_strip.clock()->setTime(_strip.clock()->time() + 1);
}

void JokerWindow::on_actionStep_time_backward_triggered()
{
	_strip.clock()->setRate(0.0);
	_strip.clock()->setTime(_strip.clock()->time() - 1);
}

void JokerWindow::on_action_3_triggered()
{
	_strip.clock()->setRate(-3.0);
}

void JokerWindow::on_action_1_triggered()
{
	_strip.clock()->setRate(-1.0);
}

void JokerWindow::on_action_0_5_triggered()
{
	_strip.clock()->setRate(-0.5);
}

void JokerWindow::on_action0_triggered()
{
	_strip.clock()->setRate(0.0);
}

void JokerWindow::on_action0_5_triggered()
{
	_strip.clock()->setRate(0.5);
}

void JokerWindow::on_action1_triggered()
{
	_strip.clock()->setRate(1.0);
}

void JokerWindow::on_action3_triggered()
{
	_strip.clock()->setRate(3.0);
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
			_strip.clock()->setTime(_doc->videoTimeIn());
	}

	fadeInMediaPanel();
}

bool JokerWindow::openVideoFile(QString videoFile)
{
	QFileInfo lastFileInfo(_doc->videoFilePath());
	QFileInfo fileInfo(videoFile);
	if (fileInfo.exists() && _videoEngine.open(videoFile)) {
		PhFrame frameIn = _videoEngine.firstFrame();

		_mediaPanel.setFirstFrame(frameIn);
		_mediaPanel.setMediaLength(_videoEngine.length());

		if(videoFile != _doc->videoFilePath()) {
			_doc->setVideoFilePath(videoFile);
			if(frameIn > 0)
				_doc->setVideoFrameIn(frameIn);
			_doc->setModified(true);
		}

		if(frameIn == 0) {
			frameIn = _doc->videoFrameIn();
			_videoEngine.setFirstFrame(frameIn);
			_videoEngine.clock()->setFrame(frameIn);
			if(fileInfo.fileName() != lastFileInfo.fileName()) {
				on_actionChange_timestamp_triggered();
				frameIn = _videoEngine.firstFrame();
			}
		}

		_videoEngine.clock()->setFrame(frameIn);

		_settings->setLastVideoFolder(fileInfo.absolutePath());
		return true;
	}
	return false;
}

void JokerWindow::timeCounter(PhTimeScale frequency)
{
	if(_strip.clock()->rate() == 1 && (Synchronizer::SyncType)_settings->synchroProtocol() != Synchronizer::NoSync) {
		_numberOfDraw++;
		if(_numberOfDraw >= frequency) {
			_numberOfDraw = 0;
			_settings->setTimePlayed(_settings->timePlayed() + 1);
		}
	}
}

void JokerWindow::on_actionChange_timestamp_triggered()
{
	hideMediaPanel();
	_strip.clock()->setRate(0);
	PhFrame frame;
	if(_synchronizer.videoClock()->frame() < _videoEngine.firstFrame())
		frame = _videoEngine.firstFrame();
	else if(_synchronizer.videoClock()->frame() > _videoEngine.firstFrame() + _videoEngine.length())
		frame = _videoEngine.lastFrame();
	else
		frame = _synchronizer.videoClock()->frame();

	PhTimeCodeDialog dlg(_strip.clock()->timeCodeType(), frame);
	if(dlg.exec() == QDialog::Accepted) {
		PhFrame frameStamp;
		if(_synchronizer.videoClock()->frame() > _videoEngine.firstFrame() + _videoEngine.length())
			frameStamp = dlg.frame() - (_videoEngine.length() - 1);
		else if (_synchronizer.videoClock()->frame() < _videoEngine.firstFrame())
			frameStamp =  dlg.frame();
		else
			frameStamp = _videoEngine.firstFrame() + dlg.frame() - _synchronizer.videoClock()->frame();

		_videoEngine.setFirstFrame(frameStamp);
		_strip.clock()->setFrame(dlg.frame());
		_doc->setVideoFrameIn(frameStamp);
		_mediaPanel.setFirstFrame(frameStamp);
		_doc->setModified(true);
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

	PhTimeCodeDialog dlg(_strip.clock()->timeCodeType(), _strip.clock()->frame());
	if(dlg.exec() == QDialog::Accepted)
		_strip.clock()->setFrame(dlg.frame());

	fadeInMediaPanel();
}

void JokerWindow::on_actionNext_element_triggered()
{
	PhTime time = _doc->nextElementTime(_strip.clock()->time());
	if(time < PHTIMEMAX)
		_strip.clock()->setTime(time);
}

void JokerWindow::on_actionPrevious_element_triggered()
{
	PhTime time = _doc->previousElementTime(_strip.clock()->time());
	if(time > PHTIMEMIN)
		_strip.clock()->setTime(time);
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
	else if(_doc->saveStripFile(fileName, _strip.clock()->timeCode()))
		_doc->setModified(false);
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
		if(_doc->saveStripFile(fileName, _strip.clock()->timeCode())) {
			_doc->setModified(false);
			setCurrentDocument(fileName);
		}
		else
			QMessageBox::critical(this, "", tr("Unable to save ") + fileName);
	}
}

bool JokerWindow::checkSaveFile()
{

	if(_doc->modified()) {
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
			if(_doc->modified())
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

void JokerWindow::on_actionForce_16_9_ratio_triggered(bool checked)
{
	_doc->setForceRatio169(checked);
	_doc->setModified(true);
}

void JokerWindow::on_actionInvert_colors_toggled(bool checked)
{
	_settings->setInvertColor(checked);
}

void JokerWindow::on_actionShow_ruler_toggled(bool checked)
{
	_settings->setDisplayRuler(checked);
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
	_videoEngine.close();
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
	_videoEngine.setDeinterlace(checked);
	if(checked != _doc->videoDeinterlace()) {
		_doc->setVideoDeinterlace(checked);
		_doc->setModified(true);
	}
}

void JokerWindow::on_actionHide_the_rythmo_triggered(bool checked)
{
	_settings->setHideStrip(checked);
}

void JokerWindow::onPaint(int width, int height)
{
	QList<PhPeople*> selectedPeoples;
	foreach(QString name, _settings->selectedPeopleNameList()) {
		PhPeople *people = _strip.doc()->peopleByName(name);
		if(people)
			selectedPeoples.append(people);
	}

	int y = 0;
	QString title = _strip.doc()->title();
	if(_strip.doc()->episode().length() > 0)
		title += " #" + _strip.doc()->episode();

	if(_settings->displayTitle() && (title.length() > 0)) {
		PhGraphicSolidRect titleBackground;
		titleBackground.setColor(QColor(0, 0, 128));
		int titleHeight = height / 40;
		titleBackground.setRect(0, y, width, titleHeight);
		int titleWidth = title.length() * titleHeight / 2;
		int titleX = (width - titleWidth) / 2;
		PhGraphicText titleText(_strip.getHUDFont());
		titleText.setColor(Qt::white);
		titleText.setRect(titleX, y, titleWidth, titleHeight);
		y += titleHeight;
		titleText.setContent(title);
		titleText.setZ(5);
		titleBackground.setZ(5);

		titleBackground.draw();
		titleText.draw();
	}

	float stripHeightRatio = 0.25f;
	if(_settings) {
		if(_settings->hideStrip())
			stripHeightRatio = 0;
		else
			stripHeightRatio = _settings->stripHeight();
	}

	int stripHeight = (height - y) * stripHeightRatio;
	int videoHeight = height - y - stripHeight;

	int tcWidth = 200 * this->devicePixelRatio();

	if((videoHeight > 0)) {
		if(_videoEngine.height() > 0) {
			int videoWidth;
			if(_doc->forceRatio169())
				videoWidth = videoHeight * 16 / 9;
			else
				videoWidth = videoHeight * _videoEngine.width() / _videoEngine.height();

			int blackStripHeight = 0; // Height of the upper black strip when video is too large
			int realVideoHeight = videoHeight;
			if(videoWidth > width) {
				videoWidth = width;
				if(_doc->forceRatio169())
					realVideoHeight = videoWidth  * 9 / 16;
				else
					realVideoHeight = videoWidth  * _videoEngine.height() / _videoEngine.width();
			}
			blackStripHeight = (height - stripHeight - realVideoHeight) / 2;

			int videoX = (width - videoWidth) / 2;
			_videoEngine.drawVideo(videoX, y + blackStripHeight, videoWidth, realVideoHeight);

			// adjust tc size
			if(videoX > tcWidth)
				tcWidth = videoX;
			else if( width < 2 * tcWidth)
				tcWidth = width / 2;
		}
		else if(_settings->displayLogo()) {
			// The logo file is 500px in native format
			int logoHeight = _videoLogo.originalSize().height();
			int logoWidth = _videoLogo.originalSize().width();
			if(videoHeight < logoHeight) {
				logoHeight = videoHeight;
				logoWidth = _videoLogo.originalSize().width() * logoHeight / _videoLogo.originalSize().height();
			}
			if(width < logoWidth) {
				logoWidth = width;
				logoHeight = _videoLogo.originalSize().height() * logoWidth / _videoLogo.originalSize().width();
			}
			_videoLogo.setRect((width - logoHeight) / 2, (videoHeight - logoHeight) / 2, logoHeight, logoHeight);
			_videoLogo.draw();

		}
	}

	PhClock *clock = _videoEngine.clock();
	long delay = (int)(24 * _settings->screenDelay() * clock->rate());
	PhTime clockTime = clock->time() + delay;
	int tcHeight = tcWidth / 5;

	int tcOffset = 0;
	if(_settings->displayNextTC())
		tcOffset = tcHeight;

	_strip.draw(0, y + videoHeight, width, stripHeight, tcOffset, selectedPeoples);
	foreach(QString info, _strip.infos()) {
		ui->videoStripView->addInfo(info);
	}

	if(_settings->displayTC()) {
		PhGraphicText tcText(_strip.getHUDFont());
		tcText.setColor(Qt::green);
		tcText.setRect(0, y, tcWidth, tcHeight);
		tcText.setContent(PhTimeCode::stringFromTime(clockTime, clock->timeCodeType()));
		tcText.draw();
	}

	if(_settings->displayNextTC()) {
		PhStripText *nextText = NULL;
		PhGraphicText nextTCText(_strip.getHUDFont());
		nextTCText.setColor(Qt::red);

		nextTCText.setRect(width - tcWidth, y, tcWidth, tcHeight);
		y += tcHeight;

		/// The next time code will be the next element of the people from the list.
		if(selectedPeoples.count()) {
			nextText = _strip.doc()->nextText(selectedPeoples, clockTime);
			if(nextText == NULL)
				nextText = _strip.doc()->nextText(selectedPeoples, 0);

			int peopleHeight = height / 30;
			PhGraphicText peopleNameText(_strip.getHUDFont());
			peopleNameText.setColor(QColor(128, 128, 128));
			foreach(PhPeople* people, selectedPeoples) {
				int peopleNameWidth = people->name().length() * peopleHeight / 2;
				peopleNameText.setRect(10, y, peopleNameWidth, peopleHeight);
				peopleNameText.setContent(people->name());
				peopleNameText.draw();
				y += peopleHeight;
			}
		}
		else {
			nextText = _strip.doc()->nextText(clockTime);
			if(nextText == NULL)
				nextText = _strip.doc()->nextText(0);
		}

		if(nextText != NULL) {
			nextTCText.setContent(PhTimeCode::stringFromTime(nextText->timeIn(), clock->timeCodeType()));
			nextTCText.draw();
		}
	}

	PhStripLoop * currentLoop = _strip.doc()->previousLoop(clockTime);
	if(currentLoop) {
		int loopNumber = currentLoop->number();
		PhGraphicText gCurrentLoop(_strip.getHUDFont(), QString::number(loopNumber));
		int loopHeight = 60;
		int loopWidth = _strip.getHUDFont()->getNominalWidth(QString::number(loopNumber)) * ((float) loopHeight / _strip.getHUDFont()->getHeight());
		gCurrentLoop.setRect(10, height - stripHeight - loopHeight, loopWidth, loopHeight);
		gCurrentLoop.setColor(Qt::blue);
		gCurrentLoop.draw();
	}

	if(_lastVideoSyncElapsed.elapsed() > 1000) {
		PhGraphicText errorText(_strip.getHUDFont(), tr("No video sync"));
		errorText.setRect(width / 2 - 100, height / 2 - 25, 200, 50);
		int red = (_lastVideoSyncElapsed.elapsed() - 1000) / 4;
		if (red > 255)
			red = 255;
		errorText.setColor(QColor(red, 0, 0));
		errorText.draw();
	}
}

void JokerWindow::onVideoSync()
{
	_lastVideoSyncElapsed.restart();
}

void JokerWindow::on_actionPrevious_loop_triggered()
{
	PhTime time = _doc->previousLoopTime(_strip.clock()->time());
	if(time > PHTIMEMIN)
		_strip.clock()->setTime(time);
}

void JokerWindow::on_actionNext_loop_triggered()
{
	PhTime time = _doc->nextLoopTime(_strip.clock()->time());
	if(time < PHTIMEMAX)
		_strip.clock()->setTime(time);
}

void JokerWindow::on_actionDisplay_the_cuts_toggled(bool checked)
{
	_settings->setDisplayCuts(checked);
}

void JokerWindow::on_actionSet_space_between_two_ruler_graduation_triggered()
{
	RulerSpaceDialog dlg(_settings);
	dlg.exec();
}
