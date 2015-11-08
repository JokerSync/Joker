/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QQmlEngine>
#include <QQmlContext>

#include "PhTools/PhDebug.h"

#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PhCommonUI/PhFeedbackDialog.h"

#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicSolidRect.h"
#include "PhGraphic/PhQmlView.h"

#include "JokerWindow.h"
#include "ui_JokerWindow.h"

#include "AboutDialog.h"
#include "PreferencesDialog.h"
#include "PeopleDialog.h"

JokerWindow::JokerWindow(JokerSettings *settings) :
	PhEditableDocumentWindow(settings),
	ui(new Ui::JokerWindow),
	_settings(settings),
	_strip(settings),
	_doc(_strip.doc()),
#ifdef USE_VIDEO
	_videoEngine(settings),
#endif
	_synchronizer(settings),
#ifdef USE_SONY
	_sonySlave(settings),
#endif
#ifdef USE_LTC
	_ltcReader(settings),
#endif
#ifdef USE_MIDI
	_mtcReader(PhTimeCodeType25),
	_mtcWriter(PhTimeCodeType25),
#endif
	_mediaPanelState(MediaPanelHidden),
	_mediaPanelAnimation(&_mediaPanel, "windowOpacity"),
	_firstDoc(true),
	_resizingStrip(false),
	_numberOfDraw(0),
	_stripTime(0)
{
	qApp->installEventFilter(this);

	// Setting up UI
	ui->setupUi(this);

	qmlRegisterType<PhQmlView>("Joker", 1, 0, "PhQmlView");

	ui->videoStripView->engine()->rootContext()->setContextProperty("doc", _doc);
	ui->videoStripView->engine()->rootContext()->setContextProperty("jokerWindow", this);
	ui->videoStripView->engine()->rootContext()->setContextProperty("selectedPeopleModel", QVariant::fromValue(_selectedPeopleList));
	ui->videoStripView->engine()->rootContext()->setContextProperty("infoModel", QVariant::fromValue(_infoList));
	ui->videoStripView->engine()->rootContext()->setContextProperty("nextPeopleModel", _strip.nextPeopleModel());
	ui->videoStripView->engine()->rootContext()->setContextProperty("stripTextModelTrack0", _strip.stripTextModelTrack0());
	ui->videoStripView->engine()->rootContext()->setContextProperty("stripTextModelTrack1", _strip.stripTextModelTrack1());
	ui->videoStripView->engine()->rootContext()->setContextProperty("stripTextModelTrack2", _strip.stripTextModelTrack2());
	ui->videoStripView->engine()->rootContext()->setContextProperty("stripTextModelTrack3", _strip.stripTextModelTrack3());
	ui->videoStripView->engine()->rootContext()->setContextProperty("verticalTimePerPixel", _settings->verticalTimePerPixel());
	ui->videoStripView->engine()->rootContext()->setContextProperty("horizontalTimePerPixel", _settings->horizontalTimePerPixel());

	ui->videoStripView->setResizeMode(QQuickWidget::SizeRootObjectToView);
	ui->videoStripView->setSource(QUrl("qrc:///Phonations/Joker/main.qml"));

	// Due to translation, Qt might not be able to link automatically the menu
	ui->actionPreferences->setMenuRole(QAction::PreferencesRole);
	ui->actionAbout->setMenuRole(QAction::AboutRole);

	connect(ui->actionFullscreen, SIGNAL(triggered()), this, SLOT(toggleFullScreen()));

	ui->videoStripView->setGraphicSettings(_settings);

	// Initialize the synchronizer
	_synchronizer.setStripClock(_strip.clock());

	// Initialize the property dialog
	_propertyDialog.setDoc(_doc);
#ifdef USE_VIDEO
	_propertyDialog.setVideoEngine(&_videoEngine);
	_synchronizer.setVideoClock(_videoEngine.clock());
#else
	ui->actionOpen_Video->setEnabled(false);
	ui->actionClose_video->setEnabled(false);
	ui->actionDeinterlace_video->setEnabled(false);
	ui->actionForce_16_9_ratio->setEnabled(false);
	ui->actionUse_native_video_size->setEnabled(false);
#endif

#ifdef USE_SONY
	connect(&_sonySlave, &PhSonySlaveController::videoSync, this, &JokerWindow::onVideoSync);
#endif

	setupSyncProtocol();

	// Setting up the media panel
	_mediaPanel.setClock(_doc->videoTimeCodeType(), _strip.clock());
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

	ui->actionDisplay_the_control_panel->setChecked(_settings->displayControlPanel());

	fadeInMediaPanel();

	// Trigger a timer that will fade off the media panel after 3 seconds
	this->connect(&_mediaPanelTimer, &QTimer::timeout, this, &JokerWindow::fadeOutMediaPanel);
	_mediaPanelTimer.start(3000);

	this->setFocus();

	ui->actionDisplay_the_information_panel->setChecked(_settings->displayNextText());

#warning /// @todo do we warn the user that test mode is on?
	ui->actionTest_mode->setChecked(_settings->stripTestMode());

#warning /// @todo move to PhDocumentWindow
	// This is for the drag and drop feature
	setAcceptDrops(true);

	ui->actionLoop->setChecked(_settings->syncLooping());

	ui->actionInvert_colors->setChecked(_settings->invertColor());

	ui->actionHide_the_rythmo->setChecked(_settings->hideStrip());

	ui->actionDisplay_the_cuts->setChecked(_settings->displayCuts());

	ui->actionDisplay_the_vertical_scale->setChecked(_settings->displayVerticalScale());

	on_actionDisplay_feet_triggered(_settings->displayFeet());

	this->connect(ui->videoStripView, &PhGraphicView::beforePaint, this, &JokerWindow::timeCounter);
	this->connect(ui->videoStripView, &PhGraphicView::beforePaint, _strip.clock(), &PhClock::elapse);

	PhQmlView *OpenGLView = ui->videoStripView->rootObject()->findChild<PhQmlView*>("PhQmlView");
	this->connect(OpenGLView, &PhQmlView::paint, this, &JokerWindow::onPaint, Qt::DirectConnection);

	_videoLogo.setFilename(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/phonations.png");
	QQuickItem *videoLogo = ui->videoStripView->rootObject()->findChild<QQuickItem*>("videoLogo");
	videoLogo->setProperty("source", QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/phonations.png"));
}

JokerWindow::~JokerWindow()
{
	_mediaPanel.close();

	delete ui;
}

void JokerWindow::closeEvent(QCloseEvent *event)
{
	// the user will be asked if the document has to be saved
	PhEditableDocumentWindow::closeEvent(event);

	// if the close operation is not cancelled by the user,
	// the media panel has to be closed manually, or the application
	// will stay open forever in the background
	if (event->isAccepted()) {
		_mediaPanel.close();
	}
}

void JokerWindow::setupSyncProtocol()
{
	PhClock* clock = NULL;
	QString mtcPortName;

	// Disable old protocol
#ifdef USE_SONY
	_sonySlave.close();
#endif
#ifdef USE_LTC
	_ltcReader.close();
#endif
#ifdef USE_MIDI
	_mtcReader.close();
	_mtcWriter.close();
#endif

	PhSynchronizer::SyncType type = (PhSynchronizer::SyncType)_settings->synchroProtocol();

	switch(type) {
#ifdef USE_SONY
	case PhSynchronizer::Sony:
		// Initialize the sony module
		if(_sonySlave.open()) {
			clock = _sonySlave.clock();
			_lastVideoSyncElapsed.start();
		}
		else {
			type = PhSynchronizer::NoSync;
			QMessageBox::critical(this, tr("Error"), tr("Unable to connect to USB422v module"));
		}
		break;
#endif
	case PhSynchronizer::LTC:
#ifdef USE_LTC
		if(_ltcReader.init(_settings->ltcInputPort()))
			clock = _ltcReader.clock();
		else {
			QMessageBox::critical(this, tr("Error"), QString(tr("Unable to open %0")).arg(_settings->ltcInputPort()));
			type = PhSynchronizer::NoSync;
		}
		break;
#endif
#ifdef USE_MIDI
	case PhSynchronizer::MTC:
		if (_settings->mtcInputUseExistingPort()) {
			mtcPortName = _settings->mtcInputPort();
		}
		else {
			mtcPortName = _settings->mtcVirtualInputPort();
		}

		if(_mtcReader.open(mtcPortName))
			clock = _mtcReader.clock();
		else {
			QMessageBox::critical(this, tr("Error"), QString(tr("Unable to open %0 midi port")).arg(mtcPortName));
			type = PhSynchronizer::NoSync;
		}
		break;
#endif
	default:
		type = PhSynchronizer::NoSync;
		break;
	}

#ifdef USE_MIDI
	if(_settings->sendMmcMessage()) {
		if(!_mtcWriter.open(_settings->mmcOutputPort())) {
			QMessageBox::critical(this, tr("Error"), QString(tr("Unable to open %0 midi port")).arg(_settings->mmcOutputPort()));
			_settings->setSendMmcMessage(false);
		}
	}
#endif

	_synchronizer.setSyncClock(clock, type);

	// Disable slide if Joker is sync to a protocol
	_mediaPanel.setSliderEnable(clock == NULL);

	_settings->setSynchroProtocol(type);
}

bool JokerWindow::openDocument(const QString &fileName)
{
	QFileInfo info(fileName);
	if(_settings->videoFileType().contains(info.suffix().toLower())) {
#ifdef USE_VIDEO
		return openVideoFile(fileName);
#else
		return false;
#endif
	}

	/// Clear the selected people name list (except for the first document).
	if(!_firstDoc)
		_settings->setSelectedPeopleNameList(QStringList());
	else
		_firstDoc = false;

	if(!_doc->openStripFile(fileName))
		return false;

	/// If the document is opened successfully :
	/// - Update the current document name (settings, windows title)
	PhEditableDocumentWindow::openDocument(fileName);
	_watcher.addPath(_doc->filePath());

#ifdef USE_VIDEO
	/// - Load the deinterlace settings
	on_actionDeinterlace_video_triggered(_doc->videoDeinterlace());
	/// - Open the corresponding video file if it exists.
	if(openVideoFile(_doc->videoFilePath())) {
		_videoEngine.setTimeIn(_doc->videoTimeIn());
		_mediaPanel.setTimeIn(_doc->videoTimeIn());
	}
	else
		_videoEngine.close();
#endif

	/// - Set the video aspect ratio.
	ui->actionForce_16_9_ratio->setChecked(_doc->forceRatio169());

	/// - Goto to the document last position.
	setCurrentTime(_doc->lastTime());
	/// - Disable the need to save flag.

	return true;
}

bool JokerWindow::eventFilter(QObject * sender, QEvent *event)
{
	/// The event filter catch the following event:
	switch (event->type()) {
	case QEvent::MouseMove: /// - Mouse move show the media panel
	case QEvent::HoverEnter:
	case QEvent::HoverMove:
		{
			fadeInMediaPanel();

			QMouseEvent * mouseEvent = (QMouseEvent*)event;
			// Check if it is near the video/strip border
			float stripHeight = this->height() * _settings->stripHeight();
			if((mouseEvent->pos().y() > (this->height() - stripHeight) - 10)
			   && (mouseEvent->pos().y() < (this->height() - stripHeight) + 10))
				QApplication::setOverrideCursor(Qt::SizeVerCursor);
			else
				QApplication::setOverrideCursor(Qt::ArrowCursor);

			if(_resizingStrip && (mouseEvent->buttons() & Qt::LeftButton)) {
				PHDEBUG << "resizing strip:" << mouseEvent->pos();
				_settings->setStripHeight(1.0 - ((float) mouseEvent->pos().y() /(float) this->height()));
			}
			break;
		}
	case QEvent::MouseButtonDblClick: /// - Double mouse click toggle fullscreen mode
		if(sender == this)
			toggleFullScreen();
		break;
	case QEvent::MouseButtonRelease:
		PHDEBUG << "end resizing strip";
		_resizingStrip = false;
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
				PHDEBUG << "start resizing strip";
				_resizingStrip = true;
			}
		}
	case QEvent::KeyPress:
		{
			QKeyEvent *keyEvent = (QKeyEvent*)event;
			if(keyEvent->key() == Qt::Key_Space) {
				on_actionPlay_pause_triggered();
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

void JokerWindow::onApplicationActivate()
{
	PhDocumentWindow::onApplicationActivate();
	fadeInMediaPanel();
}

void JokerWindow::onApplicationDeactivate()
{
	PhDocumentWindow::onApplicationDeactivate();
	hideMediaPanel();
}

void JokerWindow::on_actionOpen_triggered()
{
	hideMediaPanel();

	if(checkDocumentModification()) {
		QString filter = tr("Rythmo files") + " (";
		foreach(QString type, _settings->stripFileType())
			filter += "*." + type + " ";
		filter += ");;";
		QFileDialog dlg(this, tr("Open..."), _settings->lastDocumentFolder(), filter);

		dlg.setOption(QFileDialog::HideNameFilterDetails, false);

		dlg.setFileMode(QFileDialog::ExistingFile);
		if(dlg.exec()) {
			QString fileName = dlg.selectedFiles()[0];
			openDocument(fileName);
		}
	}
	fadeInMediaPanel();
}

void JokerWindow::on_actionPlay_pause_triggered()
{
	if(currentRate() == 0.0) {
		setCurrentRate(1.0);
	}
	else {
		setCurrentRate(0.0);
	}
}

void JokerWindow::on_actionPlay_backward_triggered()
{
	setCurrentRate(-1.0);
}

void JokerWindow::on_actionStep_forward_triggered()
{
	setCurrentRate(0.0);
	setCurrentTime(currentTime() + PhTimeCode::timePerFrame(_doc->videoTimeCodeType()));
}

void JokerWindow::on_actionStep_backward_triggered()
{
	setCurrentRate(0.0);
	setCurrentTime(currentTime() - PhTimeCode::timePerFrame(_doc->videoTimeCodeType()));
}

void JokerWindow::on_actionStep_time_forward_triggered()
{
	setCurrentRate(0.0);
	setCurrentTime(currentTime() + 1);
}

void JokerWindow::on_actionStep_time_backward_triggered()
{
	setCurrentRate(0.0);
	setCurrentTime(currentTime() - 1);
}

void JokerWindow::on_action_3_triggered()
{
	setCurrentRate(-3.0);
}

void JokerWindow::on_action_1_triggered()
{
	setCurrentRate(-1.0);
}

void JokerWindow::on_action_0_5_triggered()
{
	setCurrentRate(-0.5);
}

void JokerWindow::on_action0_triggered()
{
	setCurrentRate(0.0);
}

void JokerWindow::on_action0_5_triggered()
{
	setCurrentRate(0.5);
}

void JokerWindow::on_action1_triggered()
{
	setCurrentRate(1.0);
}

void JokerWindow::on_action3_triggered()
{
	setCurrentRate(3.0);
}

void JokerWindow::on_actionOpen_Video_triggered()
{
#ifdef USE_VIDEO
	hideMediaPanel();

	QString lastFolder = _settings->lastVideoFolder();
	QString filter = tr("Movie files") + " (";
	foreach(QString type, _settings->videoFileType())
		filter += "*." + type + " ";
	filter += ");;";

	QFileDialog dlg(this, tr("Open a video..."), lastFolder, filter);
	if(dlg.exec()) {
		QString videoFile = dlg.selectedFiles()[0];
		if(openVideoFile(videoFile))
			setCurrentTime(_doc->videoTimeIn());
	}

	fadeInMediaPanel();
#endif
}

#ifdef USE_VIDEO
bool JokerWindow::openVideoFile(QString videoFile)
{
	QFileInfo lastFileInfo(_doc->videoFilePath());
	QFileInfo fileInfo(videoFile);
	if (fileInfo.exists() && _videoEngine.open(videoFile)) {
		PhTime videoTimeIn = _videoEngine.timeIn();

		if(videoTimeIn == 0) {
			/* the video itself has no timestamp, and until now we have not
			 * propagated the doc videoTimeIn to the videoEngine */
			videoTimeIn = _doc->videoTimeIn();
			_videoEngine.setTimeIn(videoTimeIn);
			_videoEngine.clock()->setTime(videoTimeIn);

			/* ask the user if he wants to change the video time in */
			if(fileInfo.fileName() != lastFileInfo.fileName()) {
				on_actionChange_timestamp_triggered();
				videoTimeIn = _videoEngine.timeIn();
			}
		}

		if(videoFile != _doc->videoFilePath()) {
			_doc->setVideoFilePath(videoFile);
			_doc->setVideoTimeIn(videoTimeIn, timeCodeType());
			_doc->setModified(true);
		}

		_videoEngine.clock()->setTime(videoTimeIn);
		_mediaPanel.setTimeIn(videoTimeIn);
		_mediaPanel.setLength(_videoEngine.length());

		_settings->setLastVideoFolder(fileInfo.absolutePath());
		return true;
	}
	return false;
}
#endif

void JokerWindow::timeCounter(PhTime elapsedTime)
{
	int frequency = static_cast<int>(24000./static_cast<double>(elapsedTime));

	if(currentRate() == 1 && (PhSynchronizer::SyncType)_settings->synchroProtocol() != PhSynchronizer::NoSync) {
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
	setCurrentRate(0);
	PhTime time = _synchronizer.videoClock()->time();

#ifdef USE_VIDEO
	if(_synchronizer.videoClock()->time() < _videoEngine.timeIn())
		time = _videoEngine.timeIn();
	else if(_synchronizer.videoClock()->time() > _videoEngine.timeIn() + _videoEngine.length())
		time = _videoEngine.timeOut();

	PhTimeCodeDialog dlg(_doc->videoTimeCodeType(), time);
	if(dlg.exec() == QDialog::Accepted) {
		PhTime timeStamp = 0;
		if(_synchronizer.videoClock()->time() > _videoEngine.timeIn() + _videoEngine.length())
			timeStamp = dlg.time() - (_videoEngine.length() - PhTimeCode::timePerFrame(timeCodeType()));
		else if (_synchronizer.videoClock()->time() < _videoEngine.timeIn())
			timeStamp =  dlg.time();
		else
			timeStamp = _videoEngine.timeIn() + dlg.time() - _synchronizer.videoClock()->time();

		_videoEngine.setTimeIn(timeStamp);
		setCurrentTime(dlg.time());
		_doc->setVideoTimeIn(timeStamp, timeCodeType());
		_mediaPanel.setTimeIn(timeStamp);
		_doc->setModified(true);
	}
#endif

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
#ifdef USE_LTC
	QString oldLtcInputPort = _settings->ltcInputPort();
#endif // USE_LTC
#ifdef USE_MIDI
	QString oldMtcInputPort = _settings->mtcInputPort();
	QString oldMtcVirtualInputPort = _settings->mtcVirtualInputPort();
	bool oldMtcInputUseExistingPort = _settings->mtcInputUseExistingPort();
	bool oldSendMmcMessage = _settings->sendMmcMessage();
	QString oldMmcOutputPort = _settings->mmcOutputPort();
#endif // USE_MIDI

	PreferencesDialog dlg(_settings);
	if(dlg.exec() == QDialog::Accepted) {
		if((oldSynchroProtocol != _settings->synchroProtocol())
#ifdef USE_LTC
		   || (oldLtcInputPort  != _settings->ltcInputPort())
#endif // USE_LTC
#ifdef USE_MIDI
		   || (oldMtcInputPort != _settings->mtcInputPort())
		   || (oldMtcVirtualInputPort != _settings->mtcVirtualInputPort())
		   || (oldMtcInputUseExistingPort != _settings->mtcInputUseExistingPort())
		   || (oldSendMmcMessage != _settings->sendMmcMessage())
		   || (oldMmcOutputPort != _settings->mmcOutputPort())
#endif // USE_MIDI
		   ) {
			PHDEBUG << "Set protocol:" << _settings->synchroProtocol();
			setupSyncProtocol();
		}
	}

	fadeInMediaPanel();
}

void JokerWindow::fadeInMediaPanel()
{
	// Don't show the mediaPanel if Joker has not thefocus.
	if(_settings->displayControlPanel() && this->hasFocus() && ui->videoStripView->hasFocus() ) {
		_mediaPanel.show();
		_mediaPanelAnimation.stop();
		_mediaPanelAnimation.setDuration(300);
		_mediaPanelAnimation.setEndValue(1);
		_mediaPanelAnimation.setEasingCurve(QEasingCurve::InOutSine);
		_mediaPanelAnimation.start();
		_mediaPanelState = MediaPanelVisible;
		_mediaPanelTimer.start(3000);
	}
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

	PhTimeCodeDialog dlg(timeCodeType(), currentTime());
	if(dlg.exec() == QDialog::Accepted)
		setCurrentTime(dlg.time());

	fadeInMediaPanel();
}

void JokerWindow::on_actionNext_element_triggered()
{
	PhTime time = _doc->nextElementTime(currentTime());
	if(time < PHTIMEMAX)
		setCurrentTime(time);
}

void JokerWindow::on_actionPrevious_element_triggered()
{
	PhTime time = _doc->previousElementTime(currentTime());
	if(time > PHTIMEMIN)
		setCurrentTime(time);
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
	foreach(QAction * action, ui->menuOpen_recent->actions()) {
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
	if(!info.exists() || (info.suffix() != "detx"))
		on_actionSave_as_triggered();
	else if(_doc->exportDetXFile(fileName, currentTime()))
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
		if(info.suffix() != "detx")
			fileName = lastFolder + "/" + info.completeBaseName() + ".detx";
	}

	fileName = QFileDialog::getSaveFileName(this, tr("Save..."), fileName, "*.detx");
	if(fileName != "") {
		if(_doc->exportDetXFile(fileName, currentTime())) {
			_doc->setModified(false);
			PhEditableDocumentWindow::saveDocument(fileName);
		}
		else
			QMessageBox::critical(this, "", tr("Unable to save ") + fileName);
	}
}

bool JokerWindow::isDocumentModified()
{
	return _doc->modified();
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

void JokerWindow::on_actionDisplay_feet_triggered(bool checked)
{
	_settings->setDisplayFeet(checked);
}

void JokerWindow::on_actionSet_first_foot_timecode_triggered()
{
	PhTimeCodeType tcType = timeCodeType();
	PhTimeCodeDialog dlg(tcType, _settings->firstFootTime(), this);
	if(dlg.exec())
		_settings->setFirstFootTime(dlg.time());
}

void JokerWindow::on_actionNew_triggered()
{
	_doc->reset();
	on_actionClose_video_triggered();
	this->resetDocument();
}

void JokerWindow::on_actionClose_video_triggered()
{
#ifdef USE_VIDEO
	_videoEngine.close();
#endif
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
#ifdef USE_VIDEO
	_videoEngine.setDeinterlace(checked);
	if(checked != _doc->videoDeinterlace()) {
		_doc->setVideoDeinterlace(checked);
		_doc->setModified(true);
	}
#endif
}

void JokerWindow::on_actionHide_the_rythmo_triggered(bool checked)
{
	_settings->setHideStrip(checked);
}

void JokerWindow::onPaint(int width, int height)
{
#ifdef USE_VIDEO
	PhClock *clock = _videoEngine.clock();
#else
	PhClock *clock = _strip.clock();
#endif
	long delay = (int)(24 * _settings->screenDelay() * clock->rate());
	PhTime clockTime = clock->time() + delay;
	setStripTime(clockTime);

	QList<PhPeople*> selectedPeoples;
	foreach(QString name, _settings->selectedPeopleNameList()) {
		PhPeople *people = _strip.doc()->peopleByName(name);
		if(people)
			selectedPeoples.append(people);
	}

	// Note: _selectedPeopleList is needed because QVariant does not know how to handle a QList<PhPeople*>
	// we convert it to QList<QObject*>
	_selectedPeopleList.clear();
	foreach(PhPeople *people, selectedPeoples) {
		_selectedPeopleList.append(people);
	}

	// refresh the view
	// TODO the view could be refreshed more intelligently by connecting to the signals of the people selection dialog
	ui->videoStripView->engine()->rootContext()->setContextProperty("selectedPeopleModel", QVariant::fromValue(_selectedPeopleList));

	float stripHeightRatio = 0.0f;
	if(!_settings->hideStrip())
		stripHeightRatio = _settings->stripHeight();

	int stripHeight = height * stripHeightRatio;
	int videoHeight = height - stripHeight;
	int videoWidth = videoHeight * 16 / 9;
#ifdef USE_VIDEO
	if(!_doc->forceRatio169() && (_videoEngine.height() > 0))
		videoWidth = videoHeight * _videoEngine.width() / _videoEngine.height();
#endif
	int videoX = 0;
	// Center video if no information panel with next text
	if(!_settings->displayNextText())
		videoX = (width - videoWidth) / 2;

#ifdef USE_VIDEO
	// Display the video
	if((videoHeight > 0)) {
		if(_videoEngine.height() > 0) {

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

			_videoEngine.drawVideo(videoX, blackStripHeight, videoWidth, realVideoHeight);
		}
	}
#endif

	int x = videoX + videoWidth;
	int y = 0;

	QQuickItem *titleRect = ui->videoStripView->rootObject()->findChild<QQuickItem*>("titleRect");
	titleRect->setVisible(_settings->displayNextText() && (_strip.doc()->fullTitle().length() > 0));
	y += (titleRect->height() - titleRect->y())*titleRect->isVisible();

	// prepare the string list that is used to display the infos
	_infoList.clear();
	_infoList.append(QString("refresh: %1x%2, %3 / %4")
			.arg(ui->videoStripView->width())
			.arg(ui->videoStripView->height())
			.arg(ui->videoStripView->maxRefreshRate())
			.arg(ui->videoStripView->refreshRate()));
	_infoList.append(QString("Update : %1 %2").arg(ui->videoStripView->maxUpdateDuration()).arg(ui->videoStripView->lastUpdateDuration()));
	_infoList.append(QString("drop: %1 %2").arg(ui->videoStripView->dropDetected()).arg(ui->videoStripView->secondsSinceLastDrop()));
	#warning /// @todo measure fps with a custom QML element
	// The actual painting duration should be measured using a custom QML element.
	// See: http://developer.nokia.com/community/wiki/QML_Performance_Meter
	// (anyway, the QML profiler will provide much more details to the developer.)
	_infoList.append(QString("draw: N/A"));
	foreach(QString info, _strip.infos()) {
		_infoList.append(info);
	}

	ui->videoStripView->engine()->rootContext()->setContextProperty("infoModel", QVariant::fromValue(_infoList));
	QQuickItem *infoList = ui->videoStripView->rootObject()->findChild<QQuickItem*>("infoList");
	infoList->setVisible(_settings->displayInfo());

	PhStripText *nextText = NULL;
	if(_settings->displayNextText()) {
		/// The next time code will be the next element of the people from the list.
		if(selectedPeoples.count()) {
			nextText = _strip.doc()->nextText(selectedPeoples, clockTime);
			if(nextText == NULL)
				nextText = _strip.doc()->nextText(selectedPeoples, 0);
		}
		else {
			nextText = _strip.doc()->nextText(clockTime);
			if(nextText == NULL)
				nextText = _strip.doc()->nextText(0);
		}
	}

	QQuickItem *nextTcLabel = ui->videoStripView->rootObject()->findChild<QQuickItem*>("nextTcLabel");
	nextTcLabel->setVisible(_settings->displayNextText() && nextText != NULL);
	if (nextText != NULL) {
		nextTcLabel->setProperty("text", PhTimeCode::stringFromTime(nextText->timeIn(), _videoEngine.timeCodeType()));
	}
	y += nextTcLabel->height()*nextTcLabel->isVisible();

	_strip.draw(0, videoHeight, width, stripHeight, x, y, selectedPeoples);

	// FIXME the font size for the list of selected peoples is fixed, whereas it depended on the window size before
	QQuickItem *selectedPeopleList = ui->videoStripView->rootObject()->findChild<QQuickItem*>("selectedPeopleList");
	selectedPeopleList->setVisible(!_settings->hideSelectedPeoples() && _selectedPeopleList.count());

	QQuickItem *videoLogo = ui->videoStripView->rootObject()->findChild<QQuickItem*>("videoLogo");
	videoLogo->setVisible((videoHeight > 0) && (_videoEngine.height() <= 0) && _settings->displayLogo());
	videoLogo->setHeight(videoHeight);

	QQuickItem *tcLabel = ui->videoStripView->rootObject()->findChild<QQuickItem*>("tcLabel");
	tcLabel->setVisible(_settings->displayNextText());
	tcLabel->setProperty("text", PhTimeCode::stringFromTime(clockTime, _videoEngine.timeCodeType()));

	PhStripLoop * currentLoop = _strip.doc()->previousLoop(clockTime);
	setCurrentLoopLabel(currentLoop ? currentLoop->label(): "");

	QQuickItem *loopLabel = ui->videoStripView->rootObject()->findChild<QQuickItem*>("currentLoopLabel");
	loopLabel->setVisible(_settings->displayNextText());

	// placeholder used to position other UI elements
	QQuickItem *stripItem = ui->videoStripView->rootObject()->findChild<QQuickItem*>("strip");
	stripItem->setHeight(stripHeight);

	QQuickItem *noSyncLabel = ui->videoStripView->rootObject()->findChild<QQuickItem*>("noSyncLabel");
	noSyncLabel->setVisible((_settings->synchroProtocol() == PhSynchronizer::Sony) && (_lastVideoSyncElapsed.elapsed() > 1000));
	double opacity = (_lastVideoSyncElapsed.elapsed() - 1000.0d) / 1000.0d;
	noSyncLabel->setOpacity(opacity <= 0 ? 0 : opacity >= 1 ? 1 : opacity);
}

void JokerWindow::onVideoSync()
{
	_lastVideoSyncElapsed.restart();
}

void JokerWindow::on_actionPrevious_loop_triggered()
{
	PhTime time = _doc->previousLoopTime(currentTime());
	if(time > PHTIMEMIN)
		setCurrentTime(time);
}

void JokerWindow::on_actionNext_loop_triggered()
{
	PhTime time = _doc->nextLoopTime(currentTime());
	if(time < PHTIMEMAX)
		setCurrentTime(time);
}

void JokerWindow::on_actionDisplay_the_cuts_toggled(bool checked)
{
	_settings->setDisplayCuts(checked);
}

void JokerWindow::on_actionSet_distance_between_two_feet_triggered()
{
	TimeBetweenTwoFeetDialog dlg(_settings);
	dlg.exec();
}

void JokerWindow::on_actionDisplay_the_vertical_scale_triggered(bool checked)
{
	_settings->setDisplayVerticalScale(checked);
}

void JokerWindow::setCurrentTime(PhTime time)
{
	_strip.clock()->setTime(time);
#ifdef USE_MIDI
	if(_settings->sendMmcMessage())
		_mtcWriter.sendMMCGotoFromTime(time);
#endif

}

void JokerWindow::setCurrentRate(PhRate rate)
{
	_strip.clock()->setRate(rate);
#ifdef USE_MIDI
	if(_settings->sendMmcMessage()) {
		_mtcWriter.sendMMCGotoFromTime(currentTime());
		if(rate == 0.0f)
			_mtcWriter.sendMMCStop();
		else if(rate == 1.0f)
			_mtcWriter.sendMMCPlay();
	}
#endif // USE_MIDI
}

PhTime JokerWindow::currentTime()
{
	return _strip.clock()->time();
}

PhRate JokerWindow::currentRate()
{
	return _strip.clock()->rate();
}

void JokerWindow::on_actionDisplay_the_control_panel_triggered(bool checked)
{
	_settings->setDisplayControlPanel(checked);
	if(checked)
		fadeInMediaPanel();
	else
		fadeOutMediaPanel();
}

void JokerWindow::on_actionDisplay_the_information_panel_triggered(bool checked)
{
	_settings->setDisplayNextText(checked);
}

void JokerWindow::on_actionHide_selected_peoples_triggered(bool checked)
{
	_settings->setHideSelectedPeoples(checked);
}

void JokerWindow::on_actionUse_native_video_size_triggered(bool checked)
{
#ifdef USE_VIDEO
	_settings->setUseNativeVideoSize(checked);
#endif
}

PhTimeCodeType JokerWindow::timeCodeType()
{
#ifdef USE_VIDEO
	return _videoEngine.timeCodeType();
#else
#warning /// @todo Are we sure to use default 25?
	return PhTimeCodeType25;
#endif

}

void JokerWindow::on_actionSet_TC_in_triggered()
{
	_settings->setSyncLoopTimeIn(_synchronizer.time());
}

void JokerWindow::on_actionSet_TC_out_triggered()
{
	_settings->setSyncLoopTimeOut(_synchronizer.time());
}

void JokerWindow::on_actionLoop_triggered(bool checked)
{
	_settings->setSyncLooping(checked);
}

QString JokerWindow::currentLoopLabel()
{
	return _currentLoopLabel;
}

void JokerWindow::setCurrentLoopLabel(QString label)
{
	if (label != _currentLoopLabel) {
		_currentLoopLabel = label;
		emit currentLoopLabelChanged();
	}
}

PhTime JokerWindow::stripTime()
{
	return _stripTime;
}

void JokerWindow::setStripTime(PhTime time)
{
	if (time != _stripTime) {
		_stripTime = time;
		emit stripTimeChanged();
	}
}
