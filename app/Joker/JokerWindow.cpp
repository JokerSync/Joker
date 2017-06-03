/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QCloseEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QFontDatabase>

#include "PhTools/PhDebug.h"

#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PhCommonUI/PhFeedbackDialog.h"

#include "PhGraphic/PhGraphicText.h"
#include "PhGraphic/PhGraphicSolidRect.h"

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
	_secondScreenWindow(NULL),
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
	_firstDoc(true),
	_resizingStrip(false),
	_setCurrentTimeToVideoTimeIn(false),
	_syncTimeInToDoc(false),
	_timePlayed(settings->timePlayed())
{
	// Setting up UI
	ui->setupUi(this);

	QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/Bookerly-BoldItalic.ttf");
	QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath() + PATH_TO_RESSOURCES + "/Cappella-Regular.ttf");

	// Due to translation, Qt might not be able to link automatically the menu
	ui->actionPreferences->setMenuRole(QAction::PreferencesRole);
	ui->actionAbout->setMenuRole(QAction::AboutRole);

	connect(ui->actionFullscreen, &QAction::triggered, this, &JokerWindow::toggleFullScreen);

	this->restoreGeometry(_settings->windowGeometry());
	_mediaPanel.move(this->x() + this->width() / 2 - _mediaPanel.width() / 2,
	                 this->y() + this->height() * 2 / 3);

	ui->videoStripView->setGraphicSettings(_settings);

	// Initialize the synchronizer
	_synchronizer.setStripClock(_strip.clock());

	// Initialize the property dialog
	_propertyDialog.setDoc(_doc);
#ifdef USE_VIDEO
	_propertyDialog.setVideoEngine(&_videoEngine);
	_synchronizer.setVideoClock(_videoEngine.clock());
	connect(&_videoEngine, &PhVideoEngine::timeCodeTypeChanged, this, &JokerWindow::onTimecodeTypeChanged);
	ui->actionPicture_in_picture->setChecked(_settings->videoPictureInPicture());
	ui->actionSecond_screen->setChecked(_settings->videoSecondScreen());
	if(_settings->videoSecondScreen())
		on_actionSecond_screen_triggered(true);
#else
	ui->actionOpen_Video->setEnabled(false);
	ui->actionClose_video->setEnabled(false);
	ui->actionDeinterlace_video->setEnabled(false);
	ui->actionForce_16_9_ratio->setEnabled(false);
	ui->actionUse_native_video_size->setEnabled(false);
	ui->actionPicture_in_picture->setEnabled(false);
#endif

#ifdef USE_SONY
	connect(&_sonySlave, &PhSonySlaveController::videoSync, this, &JokerWindow::onVideoSync);
#endif

#ifdef USE_LTC
	connect(&_ltcReader, &PhLtcReader::timeCodeTypeChanged, this, &JokerWindow::onTimecodeTypeChanged);
#endif

#ifdef USE_MIDI
	_mtcReader.force24as2398(_settings->mtcForce24as2398());
	connect(&_mtcReader, &PhMidiTimeCodeReader::timeCodeTypeChanged, this, &JokerWindow::onTimecodeTypeChanged);
#endif // USE_MIDI

	setupSyncProtocol();

	// Setting up the media panel
	_mediaPanel.setClock(_strip.clock());
	_mediaPanel.setDropdownEnable(false);
	onTimecodeTypeChanged(this->synchroTimeCodeType());

	ui->actionDisplay_the_control_panel->setChecked(_settings->displayControlPanel());

	showMediaPanel();

	this->setFocus();

	ui->actionDisplay_the_information_panel->setChecked(_settings->displayNextText());

	ui->actionTest_mode->setChecked(_settings->stripTestMode());

	ui->actionLoop->setChecked(_settings->syncLooping());

	ui->actionInvert_colors->setChecked(_settings->invertColor());

	ui->actionHide_the_rythmo->setChecked(_settings->hideStrip());

	ui->actionDisplay_the_cuts->setChecked(_settings->displayCuts());

	ui->actionDisplay_the_vertical_scale->setChecked(_settings->displayVerticalScale());

	on_actionDisplay_feet_triggered(_settings->displayFeet());

	this->connect(ui->videoStripView, &PhGraphicView::beforePaint, this, &JokerWindow::timeCounter);
	this->connect(ui->videoStripView, &PhGraphicView::beforePaint, _strip.clock(), &PhClock::elapse);

	this->connect(ui->videoStripView, &PhGraphicView::paint, this, &JokerWindow::onPaint);

	_videoLogo.setFilename(":/Joker/phonations");
	_videoLogo.setTransparent(true);

#ifdef USE_VIDEO
	this->connect(&_videoEngine, &PhVideoEngine::opened, this, &JokerWindow::videoFileOpened);
#endif

//	_playButton.setFilename(":/PhCommonUI/play");
//	_playButton.setTransparent(true);
//	_forwardButton.setFilename(":/PhCommonUI/forward");
//	_forwardButton.setTransparent(true);
}

JokerWindow::~JokerWindow()
{
	_mediaPanel.close();

	delete ui;
}

void JokerWindow::closeEvent(QCloseEvent *event)
{
	// The user will be asked if the document has to be saved
	PhEditableDocumentWindow::closeEvent(event);

	// If the close operation is not cancelled by the user
	if (event->isAccepted()) {
		// The media panel has to be closed manually, or the application
		// will stay open forever in the background
		_mediaPanel.close();

		if(_secondScreenWindow) {
			_secondScreenWindow->close();
			delete _secondScreenWindow;
			_secondScreenWindow = NULL;
		}

		this->disconnect(ui->videoStripView, &PhGraphicView::beforePaint, this, &JokerWindow::timeCounter);
		this->disconnect(ui->videoStripView, &PhGraphicView::beforePaint, _strip.clock(), &PhClock::elapse);

		this->disconnect(ui->videoStripView, &PhGraphicView::paint, this, &JokerWindow::onPaint);


		// Force doc to unmodified to avoid double confirmation
		// since closeEvent is called twice
		// https://bugreports.qt.io/browse/QTBUG-43344
		_doc->setModified(false);

		_settings->setTimePlayed(_timePlayed);
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
			onTimecodeTypeChanged((PhTimeCodeType)_settings->sonySlaveCommunicationTimeCodeType());
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
	if(!_firstDoc && (fileName != _settings->currentDocument()))
		_settings->setSelectedPeopleNameList(QStringList());
	else
		_firstDoc = false;

	if(!_doc->openStripFile(fileName))
		return false;

	/// If the document is opened successfully :
	/// - Update the current document name (settings, windows title)
	PhEditableDocumentWindow::openDocument(fileName);
	if(fileName != _doc->filePath()) {
		PHDEBUG << "Adding to watch: " << _doc->filePath();
		_watcher.addPath(_doc->filePath());
	}

#ifdef USE_VIDEO
	/// - Load the deinterlace settings
	on_actionDeinterlace_video_triggered(_doc->videoDeinterlace());
	/// - Open the corresponding video file if it exists.
	this->checkFilePermission(_doc->videoFilePath());
	_syncTimeInToDoc = true;
	openVideoFile(_doc->videoFilePath());
#endif

	return true;
}

void JokerWindow::saveDocument(const QString &fileName)
{
	if(_doc->exportDetXFile(fileName, currentTime())) {
		_doc->setModified(false);
		PhEditableDocumentWindow::saveDocument(fileName);
	}
	else
		QMessageBox::critical(this, "", QString(tr("Unable to save %1")).arg(fileName));
}

void JokerWindow::onExternalChange(const QString &path)
{
	PhTime currentTime = _videoEngine.clock()->time();
	PhDocumentWindow::onExternalChange(path);
	_videoEngine.clock()->setTime(currentTime);
}

bool JokerWindow::eventFilter(QObject * sender, QEvent *event)
{
	/// The event filter catch the following event:
	switch (event->type()) {
	case QEvent::MouseMove: /// - Mouse move show the media panel
	case QEvent::HoverEnter:
	case QEvent::HoverMove:
		{
			QMouseEvent * mouseEvent = (QMouseEvent*)event;
			// Check if it is near the video/strip border
			float stripHeight = this->height() * _settings->stripHeight();
			if((mouseEvent->pos().y() > (this->height() - stripHeight) - 10)
			   && (mouseEvent->pos().y() < (this->height() - stripHeight) + 10))
				QApplication::setOverrideCursor(Qt::SizeVerCursor);
			else
				QApplication::setOverrideCursor(Qt::ArrowCursor);

			if(_resizingStrip && (mouseEvent->buttons() & Qt::LeftButton)) {
				float newStripHeight = 1.0 - ((float) mouseEvent->pos().y() /(float) this->height());
				if(newStripHeight <= 0) {
					_settings->resetStripHeight();
					ui->actionHide_the_rythmo->setChecked(true);
					_settings->setHideStrip(true);
				}
				else {
					PHDBG(2) << "resizing strip:" << newStripHeight;
					_settings->setStripHeight(newStripHeight);
				}
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
			break;
		}
	case QEvent::KeyPress:
		{
			QKeyEvent *keyEvent = (QKeyEvent*)event;
			if(keyEvent->key() == Qt::Key_Space) {
				on_actionPlay_pause_triggered();
			}
			break;
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
	showMediaPanel();
}

void JokerWindow::onApplicationDeactivate()
{
	PhDocumentWindow::onApplicationDeactivate();
	hideMediaPanel();
}

void JokerWindow::showMediaPanel()
{
	// Don't show the mediaPanel if Joker has not thefocus.
	if(_settings->displayControlPanel()) {
		_mediaPanel.show();
	}
}

void JokerWindow::hideMediaPanel()
{
	_mediaPanel.hide();
}

void JokerWindow::onPaint(int width, int height)
{
#ifdef USE_VIDEO
	PhClock *clock = _videoEngine.clock();
#else
	PhClock *clock = _strip.clock();
#endif
	PhTime delay = (PhTime)(24 * _settings->screenDelay() * clock->rate());
	PhTime clockTime = clock->time() + delay;

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
	int videoAvailableWidth = width;
	// Center video if no information panel with next text
	if(_settings->displayNextText())
		videoAvailableWidth = width * 0.8f;
	QColor infoColor = _settings->backgroundColorLight();

	int y = 0;
#ifdef USE_VIDEO
	// Display the video
	if((videoHeight > 0)) {
		if(_videoEngine.height() > 0) {

			int blackStripHeight = 0; // Height of the upper black strip when video is too large
			int realVideoHeight = videoHeight;
			if(videoWidth > videoAvailableWidth) {
				videoWidth = videoAvailableWidth;
				if(_doc->forceRatio169())
					realVideoHeight = videoWidth  * 9 / 16;
				else
					realVideoHeight = videoWidth  * _videoEngine.height() / _videoEngine.width();
			}
			blackStripHeight = (height - stripHeight - realVideoHeight) / 2;

			int videoX = (videoAvailableWidth - videoWidth) / 2;

			_videoEngine.drawVideo(videoX, blackStripHeight, videoWidth, realVideoHeight, delay);

			if(_settings->videoPictureInPicture()) {
				int pipWidth = videoWidth * _settings->videoPictureInPictureRatio();
				int pipHeight = realVideoHeight * _settings->videoPictureInPictureRatio();
				int pipX = 0;
				if(_settings->videoPictureInPicturePositionRight()) {
					pipX = width - pipWidth;
					y += pipHeight;
				}
				_videoEngine.drawVideo(pipX, 0, pipWidth, pipHeight,
				                       delay + 24 * _settings->videoPictureInPictureOffset());
			}
		}
		else if(_settings->displayLogo()) {
			// The logo file is 500px in native format
			int logoHeight = _videoLogo.originalSize().height();
			int logoWidth = _videoLogo.originalSize().width();
			int logoMaxHeight = videoHeight * 9 / 10;
			if(logoHeight > logoMaxHeight) {
				logoHeight = logoMaxHeight;
				logoWidth = _videoLogo.originalSize().width() * logoHeight / _videoLogo.originalSize().height();
			}
			if(logoWidth > videoAvailableWidth) {
				logoWidth = videoAvailableWidth;
				logoHeight = _videoLogo.originalSize().height() * logoWidth / _videoLogo.originalSize().width();
			}
			_videoLogo.setRect((videoAvailableWidth - logoWidth) / 2, (logoMaxHeight - logoHeight) / 2, logoWidth, logoHeight);
			_videoLogo.draw();

			QFileInfo videoFileInfo(_doc->videoFilePath());
			QString videoBaseName = videoFileInfo.fileName();
			PhGraphicText gVideoFileName(_strip.hudFont(), videoBaseName);
			int videoBaseHeight = (videoHeight - logoMaxHeight) / 2;
			int videoBaseNameNominalWidth = _strip.hudFont()->getNominalWidth(videoBaseName);
			if(videoBaseNameNominalWidth > 0) {
				int videoBaseWidth = videoBaseNameNominalWidth * videoBaseHeight / 110;
				if (videoBaseWidth > videoAvailableWidth) {
					videoBaseWidth = videoAvailableWidth;
					videoBaseHeight = videoBaseWidth * 110 / videoBaseNameNominalWidth;
				}
				gVideoFileName.setColor(infoColor);
				gVideoFileName.setWidth(videoBaseWidth);
				gVideoFileName.setHeight(videoBaseHeight);
				gVideoFileName.setX((videoAvailableWidth - videoBaseWidth) / 2);
				gVideoFileName.setY(logoMaxHeight + (videoHeight - logoMaxHeight) / 2 - videoBaseHeight / 2);
				gVideoFileName.draw();
			}
		}
	}
#endif

	// Get the selected people list
	QList<PhPeople*> selectedPeoples;
	foreach(QString name, _settings->selectedPeopleNameList()) {
		PhPeople *people = _strip.doc()->peopleByName(name);
		if(people)
			selectedPeoples.append(people);
	}

	int x = videoAvailableWidth;
	if(_settings->displayNextText()) {
		int infoWidth = width - videoAvailableWidth;
		int spacing = 4;

		// Display the title
		{
			QString title = _strip.doc()->title().toLower();
			if(_strip.doc()->episode().length() > 0)
				title += " #" + _strip.doc()->episode().toLower();

			int titleHeight = infoWidth / 12;
			int titleWidth = _strip.hudFont()->getNominalWidth(title) * titleHeight / 110;
			PhGraphicText titleText(_strip.hudFont());
			titleText.setColor(infoColor);
			titleText.setRect(x + spacing, y, titleWidth, titleHeight);
			y += titleHeight;
			titleText.setContent(title);
			titleText.setZ(5);
			titleText.draw();
		}

		// Display the current timecode
		{
			int tcWidth = infoWidth - 2 * spacing;
			int tcHeight = infoWidth / 6;
			PhGraphicText tcText(_strip.hudFont());
			tcText.setColor(infoColor);
			tcText.setRect(x + 4, y, tcWidth, tcHeight);
			tcText.setContent(PhTimeCode::stringFromTime(clockTime, localTimeCodeType()));
			tcText.draw();

			y += tcHeight;
		}

		// Display the box around current loop number
		int boxWidth = infoWidth / 4;
		int nextTcWidth = infoWidth - boxWidth - 3 * spacing;
		int nextTcHeight = nextTcWidth / 6;
		int boxHeight = nextTcHeight + spacing;
		{
			PhGraphicSolidRect outsideLoopRect(x + spacing, y, boxWidth, boxHeight);
			outsideLoopRect.setColor(infoColor);
			outsideLoopRect.draw();

			// Display the current loop number
			QString loopLabel = "0";
			PhStripLoop * currentLoop = _strip.doc()->previousLoop(clockTime);
			if(currentLoop)
				loopLabel = currentLoop->label();
			int loopWidth = _strip.hudFont()->getNominalWidth(loopLabel) * nextTcHeight / 110;
			int loopHeight = nextTcHeight;
			int loopX = x + spacing + (boxWidth - loopWidth) / 2;
			int loopY = y + spacing;

			PhGraphicText gCurrentLoop(_strip.hudFont(), loopLabel);

			gCurrentLoop.setRect(loopX, loopY, loopWidth, loopHeight);
			gCurrentLoop.setColor(Qt::black);
			gCurrentLoop.draw();
		}

		// Display the next timecode
		{
			/// The next time code will be the next element of the people from the list.
			PhStripText *nextText = NULL;
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

			PhTime nextTextTime = 0;
			if(nextText != NULL)
				nextTextTime = nextText->timeIn();

			PhGraphicText nextTCText(_strip.hudFont());
			nextTCText.setColor(infoColor);

			int nextTcX = x + 2 * spacing + boxWidth;
			int nextTcY = y + spacing;
			nextTCText.setRect(nextTcX, nextTcY, nextTcWidth, nextTcHeight);

			nextTCText.setContent(PhTimeCode::stringFromTime(nextTextTime, localTimeCodeType()));
			nextTCText.draw();

			y += boxHeight + spacing;
		}

		// Display the control button
#warning /// @todo Add control button to the interface
//		{
//			_playButton.setColor(infoColor);
//			_playButton.setRect(x, y, 16, 16);
//			_playButton.draw();
//			_forwardButton.setColor(infoColor);
//			_forwardButton.setRect(x + 16, y, 16, 16);
//			_forwardButton.draw();
//		}
	}

	if(stripHeight > 0) {
		if(_synchronizer.stripClock() == NULL) {
			PHDEBUG <<  "Reconnect the strip clock";
			_synchronizer.setStripClock(_strip.clock());
#ifdef USE_VIDEO
			_mediaPanel.setClock(_strip.clock());
#endif
		}

		_strip.draw(0, videoHeight, width, stripHeight, x, y, selectedPeoples);
	}
	else if(_synchronizer.stripClock()) {
		PHDEBUG << "Disconnect the strip clock";
		_synchronizer.setStripClock(NULL);
#ifdef USE_VIDEO
		_mediaPanel.setClock(_videoEngine.clock());
#endif
	}

	foreach(QString info, _strip.infos()) {
		ui->videoStripView->addInfo(info);
	}

	if((_settings->synchroProtocol() == PhSynchronizer::Sony) && (_lastVideoSyncElapsed.elapsed() > 1000)) {
		PhGraphicText errorText(_strip.hudFont(), tr("No video sync"));
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

void JokerWindow::onTimecodeTypeChanged(PhTimeCodeType)
{
	PHDEBUG << this->synchroTimeCodeType();
	_mediaPanel.setTimeCodeType(this->synchroTimeCodeType());
}

PhTimeCodeType JokerWindow::localTimeCodeType()
{
#ifdef USE_VIDEO
	return _videoEngine.timeCodeType();
#else
	return _doc->videoTimeCodeType();
#endif

}

PhTimeCodeType JokerWindow::synchroTimeCodeType()
{
	switch (_settings->synchroProtocol()) {
#ifdef USE_SONY
	case PhSynchronizer::Sony:
		return _sonySlave.timeCodeType();
#endif
#ifdef USE_LTC
	case PhSynchronizer::LTC:
		return _ltcReader.timeCodeType();
#endif
#ifdef USE_MIDI
	case PhSynchronizer::MTC:
		return _mtcReader.timeCodeType();
#endif
	default:
		return localTimeCodeType();
	}
}

void JokerWindow::on_actionOpen_triggered()
{
	hideMediaPanel();

	if(checkDocumentModification()) {
		QString filter = tr("Rythmo files") + " (";
		foreach(QString type, _settings->stripFileType())
			filter += "*." + type + " ";
		filter += ")";
		QFileDialog dlg(this, tr("Open..."), _settings->lastDocumentFolder(), filter);

		dlg.setFileMode(QFileDialog::ExistingFile);
		if(dlg.exec()) {
			QString fileName = dlg.selectedFiles()[0];
			if(!openDocument(fileName))
				QMessageBox::critical(this, "", QString(tr("Unable to open %1")).arg(fileName));
		}
	}
	showMediaPanel();
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
	setCurrentTime(currentTime() + PhTimeCode::timePerFrame(localTimeCodeType()));
}

void JokerWindow::on_actionStep_backward_triggered()
{
	setCurrentRate(0.0);
	setCurrentTime(currentTime() - PhTimeCode::timePerFrame(localTimeCodeType()));
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
	filter += ");";

	QFileDialog dlg(this, tr("Open a video..."), lastFolder, filter);
	if(dlg.exec()) {
		QString videoFile = dlg.selectedFiles()[0];

		_setCurrentTimeToVideoTimeIn = true;
		openVideoFile(videoFile);
	}

	showMediaPanel();
#endif
}

#ifdef USE_VIDEO
void JokerWindow::videoFileOpened(bool success)
{
	PHDEBUG << "video file opened";

	if (success) {
		PhTime videoTimeIn = _videoEngine.timeIn();
		QFileInfo lastFileInfo(_doc->videoFilePath());
		QFileInfo fileInfo(_videoEngine.fileName());

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

		if(_videoEngine.fileName() != _doc->videoFilePath()) {
			_doc->setVideoFilePath(_videoEngine.fileName());
			_doc->setVideoTimeIn(videoTimeIn, _videoEngine.timeCodeType());
			_doc->setModified(true);
		}

		_videoEngine.clock()->setTime(videoTimeIn);
		_mediaPanel.setTimeIn(videoTimeIn);
		_mediaPanel.setLength(_videoEngine.length());

		_settings->setLastVideoFolder(fileInfo.absolutePath());

		this->addFilePermission(_doc->videoFilePath());

		if (_setCurrentTimeToVideoTimeIn) {
			setCurrentTime(_doc->videoTimeIn());
			_setCurrentTimeToVideoTimeIn = false;
		}

		if (_syncTimeInToDoc) {
			_videoEngine.setTimeIn(_doc->videoTimeIn());
			_mediaPanel.setTimeIn(_doc->videoTimeIn());
		}
	}

	if (_syncTimeInToDoc) {
		/// - Set the video aspect ratio.
		ui->actionForce_16_9_ratio->setChecked(_doc->forceRatio169());

		/// - Goto to the document last position.
		setCurrentTime(_doc->lastTime());

		_syncTimeInToDoc = false;
	}
}

bool JokerWindow::openVideoFile(QString videoFile)
{
	QFileInfo fileInfo(videoFile);
	if (fileInfo.exists() && _videoEngine.open(videoFile)) {
		return true;
	}
	return false;
}
#endif

void JokerWindow::timeCounter(PhTime elapsedTime)
{
	if(currentRate() == 1 && (PhSynchronizer::SyncType)_settings->synchroProtocol() != PhSynchronizer::NoSync) {
		_timePlayed += elapsedTime;
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

	PhTimeCodeDialog dlg(localTimeCodeType(), time);
	if(dlg.exec() == QDialog::Accepted) {
		PhTime timeStamp = 0;
		if(_synchronizer.videoClock()->time() > _videoEngine.timeIn() + _videoEngine.length())
			timeStamp = dlg.time() - (_videoEngine.length() - PhTimeCode::timePerFrame(localTimeCodeType()));
		else if (_synchronizer.videoClock()->time() < _videoEngine.timeIn())
			timeStamp =  dlg.time();
		else
			timeStamp = _videoEngine.timeIn() + dlg.time() - _synchronizer.videoClock()->time();

		_videoEngine.setTimeIn(timeStamp);
		setCurrentTime(dlg.time());
		_doc->setVideoTimeIn(timeStamp, localTimeCodeType());
		_mediaPanel.setTimeIn(timeStamp);
		_doc->setModified(true);
	}
#endif

	showMediaPanel();
}



void JokerWindow::on_actionAbout_triggered()
{
	hideMediaPanel();

	AboutDialog dlg;
	dlg.setTimePlayed(_timePlayed);
	dlg.exec();

	showMediaPanel();
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
#ifdef USE_MIDI
		_mtcReader.force24as2398(_settings->mtcForce24as2398());
#endif // USE_MIDI
	}

	showMediaPanel();
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

	PhTimeCodeDialog dlg(localTimeCodeType(), currentTime());
	if(dlg.exec() == QDialog::Accepted)
		setCurrentTime(dlg.time());

	showMediaPanel();
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
	else
		saveDocument(fileName);
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
	if(fileName != "")
		saveDocument(fileName);
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
	PhTimeCodeDialog dlg(localTimeCodeType(), _settings->firstFootTime(), this);
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
	showMediaPanel();
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
		showMediaPanel();
	else
		hideMediaPanel();
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

void JokerWindow::on_actionPicture_in_picture_triggered(bool checked)
{
#ifdef USE_VIDEO
	_settings->setVideoPictureInPicture(checked);
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

void JokerWindow::on_actionSecond_screen_triggered(bool checked)
{
#ifdef USE_VIDEO
	_settings->setVideoSecondScreen(checked);
	if(checked) {
		_secondScreenWindow = new SecondScreenWindow(&_videoEngine, ui->videoStripView, _settings);
		_secondScreenWindow->show();
		connect(_secondScreenWindow, &SecondScreenWindow::closing, this, &JokerWindow::onSecondScreenClosed);
	}
	else {
		_secondScreenWindow->close();
		delete _secondScreenWindow;
		_secondScreenWindow = NULL;
	}
#endif
}

void JokerWindow::onSecondScreenClosed(bool closedFromUser)
{
	ui->actionSecond_screen->setChecked(false);
	if(_secondScreenWindow) {
		delete _secondScreenWindow;
		_secondScreenWindow = NULL;
	}
	if(closedFromUser)
		_settings->setVideoSecondScreen(false);
}
