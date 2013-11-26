#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QFontDialog>
#include <QFileOpenEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QXmlStreamWriter>
#include <QDomDocument>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"
#include "PhCommonUI/PhFontDialog.h"
#include "AboutMenu.h"
#include "PreferencesDialog.h"

MainWindow::MainWindow(QSettings *settings) :
	QMainWindow(NULL),
	ui(new Ui::MainWindow),
	_settings(settings),
	_sonySlave(PhTimeCodeType25, settings),
	_mediaPanelAnimation(&_mediaPanel, "windowOpacity")
{
	// Setting up UI
	ui->setupUi(this);

	setupOpenRecentMenu();

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

	// Initialize the sony module
	if(_settings->value("sonyAutoConnect", true).toBool())
	{
		if(_sonySlave.open())
		{
			_synchronizer.setSonyClock(_sonySlave.clock());
			ui->videoStripView->setSony(&_sonySlave);
		}
		else
			QMessageBox::critical(this, "Sony Test", "Unable to connect to Sony slave");
	}

	// Setting up the media panel
	_mediaPanel.setClock(_strip->clock());
	_mediaPanel.show();
	_mediaPanelState = MediaPanelVisible;

	// Trigger a timer that will fade off the media panel after 3 seconds
	this->connect(&_mediaPanelTimer, SIGNAL(timeout()), this, SLOT(fadeOutMediaPanel()));
	_mediaPanelTimer.start(3000);

	// Set up a filter for catching mouse move event (see eventFilter())
	// that will show the media panel back
	qApp->installEventFilter(this);

	this->setFocus();

	if(_settings->value("stripTestMode").toBool())
	{
#warning TODO do we warn the user that test mode is on?
		ui->actionTest_mode->setChecked(true);
	}

#warning TODO fix fullscreen on startup
	if(_settings->value("startFullScreen", false).toBool())
		this->showFullScreen();

	// This is for the drag and drop feature
	setAcceptDrops(true);

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openRecent()
{
	// This slot is triggered by dynamic action
	openFile(sender()->objectName());
}

void MainWindow::updateOpenRecent()
{
	if(!ui->menuOpen_recent->isEnabled())
		ui->menuOpen_recent->setEnabled(true);

	QAction * menu = QObject::findChild<QAction *>(_doc->getFilePath(), Qt::FindDirectChildrenOnly);
	// If the object already belong to the list, set it on top
	if(menu)
	{
		ui->menuOpen_recent->removeAction(menu);
		ui->menuOpen_recent->insertAction(ui->menuOpen_recent->actions().first(), menu);
	}
	// Else, add it
	else
	{
		// Set the corresponding button
		QAction * action = new QAction(_doc->getFilePath(), this);
		// Set the ObjectName, very important for openRecent()
		action->setObjectName(_doc->getFilePath());
		connect(action, SIGNAL(triggered()), this, SLOT(openRecent()));
		// Insert it in the button list
		_recentFileButtons.insert(0, action);
		// Insert it at the first place on the menu
		ui->menuOpen_recent->insertAction(ui->menuOpen_recent->actions().first(), action);
	}

	// Open the settings group of recent files
	_settings->beginGroup("openRecent");
	int i = 1;
	// Rewrite the setting files from the menu items
	foreach(QAction * action, ui->menuOpen_recent->actions())
	{
		// Break if the separator or the max number is reached
		if(action->isSeparator() or i > 10)
			break;
		// Write the setting
		_settings->setValue(QString::number(i), action->objectName());
		i++;
	}
	// Close the setting group
	_settings->endGroup();
}

void MainWindow::setupOpenRecentMenu()
{
	// Open the settings group of recent files
	_settings->beginGroup("openRecent");
	// List them
	QStringList recentList = _settings->childKeys();
	foreach(QString recentFileIndex, recentList)
	{
		QString fileName = _settings->value(recentFileIndex, "empty").toString();
		QAction * action = new QAction(fileName, this);
		action->setObjectName(fileName);
		connect(action, SIGNAL(triggered()), this, SLOT(openRecent()));
		_recentFileButtons.append(action);
	}

	// Add all the actions to the menu
	ui->menuOpen_recent->insertActions(ui->menuOpen_recent->actions().last(), _recentFileButtons.toList());
	// Add a separator just above the "clear list" menu
	ui->menuOpen_recent->insertAction(ui->menuOpen_recent->actions().last(), ui->menuOpen_recent->addSeparator());
	// Close the group
	_settings->endGroup();

}

void MainWindow::openFile(QString fileName)
{
	hideMediaPanel();

	PHDEBUG << "openFile : " << fileName;
	//PhString fileName = QFileDialog::getOpenFileName(this, tr("Open a script"),QDir::homePath(), "Script File (*.detx)");

	// Checking if the file exists
	if(QFile::exists(fileName))
	{
		// Try to open the document
		if(fileName.split(".").last() == "detx")
		{
			if(_doc->openDetX(fileName))
			{
				// On succeed, synchronizing the clocks
				_strip->clock()->setTimeCodeType(_doc->getTCType());
				_strip->clock()->setFrame(_doc->getLastFrame());
				this->setWindowTitle(fileName);
				if(openVideoFile(_doc->getVideoPath()))
					ui->actionSave->setEnabled(true);
				_settings->setValue("lastfile", fileName);
			}
		}
		else if(fileName.split(".").last() == "strip")
		{
			openStripFile(fileName);
			ui->actionSave->setEnabled(true);
			_settings->setValue("lastfile", fileName);
		}

		updateOpenRecent();
	}
}

bool MainWindow::openStripFile(QString stripFileName)
{
	QFile xmlFile(stripFileName);
	if(!xmlFile.open(QIODevice::ReadOnly))
	{
		PHDEBUG << "Unable to open" << stripFileName;
		return false;
	}

	// Loading the DOM
	QDomDocument *domDoc = new QDomDocument();
	if (!domDoc->setContent(&xmlFile))
	{
		xmlFile.close();
		PHDEBUG << "The XML document seems to be bad formed" << stripFileName;
		return false;
	}

	PHDEBUG << ("Start parsing " + stripFileName);
	QDomElement stripDocument = domDoc->documentElement();

	if(stripDocument.tagName() != "strip")
	{
		xmlFile.close();
		PHDEBUG << "Bad root element :" << stripDocument.tagName();
		return false;
	}

	QDomElement metaInfo =	stripDocument.elementsByTagName("meta").at(0).toElement();
	// Reading the header
	if(stripDocument.elementsByTagName("meta").count())
	{
		for(int i = 0; i < stripDocument.elementsByTagName("media").count(); i++)
		{
			QDomElement line = metaInfo.elementsByTagName("media").at(i).toElement();
			PHDEBUG << "line" << line.attribute("type");
			if(line.attribute("type") == "detx")
			{
				if(_doc->openDetX(line.text()))
				{
					PHDEBUG << "On open detx";
					// On succeed, synchronizing the clocks
					_strip->clock()->setTimeCodeType(_doc->getTCType());
					_strip->clock()->setFrame(_doc->getLastFrame());
					this->setWindowTitle(line.text());
				}
			}
			if(line.attribute("type")  == "video")
			{
				PHDEBUG << "On Video Open" << metaInfo.elementsByTagName("state").at(0).toElement().attribute("lastTimeCode");
				openVideoFile(line.text());
				_strip->clock()->setFrame(PhTimeCode::frameFromString(metaInfo.elementsByTagName("state").at(0).toElement().attribute("lastTimeCode"), _doc->getTCType()));

			}
//			else
//			{
//				PHDEBUG << "settimming";
//				_strip->clock()->setFrame(PhTimeCode::frameFromString(line.attribute("lastTimeCode"), _doc->getTCType()));
//			}
		}
		_settings->setValue("lastfile", stripFileName);
	}

}

bool MainWindow::eventFilter(QObject *sender, QEvent *event)
{
	// The used variable must be declared out of the switch
	QString filePath;
	const QMimeData* mimeData;

	switch (event->type()) {
	case QEvent::FileOpen:
		filePath = static_cast<QFileOpenEvent *>(event)->file();
		// As the plist file list all the supported format (which are .detx, .avi & .mov)
		// if the file is not a detx file, it's a video file, we don't need any protection
		if(filePath.split(".").last().toLower() == "detx")
			openFile(filePath);
		else
			openVideoFile(filePath);
		break;

		// Hide and show the mediaPanel
	case QEvent::MouseMove:
		if(this->hasFocus())
			fadeInMediaPanel();
		break;

	case QEvent::Drop:
		mimeData = static_cast<QDropEvent *>(event)->mimeData();

		// If there is one file (not more) we open it
		if (mimeData->urls().length() == 1)
		{
			QString filePath = mimeData->urls().first().toLocalFile();
			QString fileType = filePath.split(".").last().toLower();
			if(fileType == "detx" or fileType == "strip")
				openFile(filePath);
			else if (fileType == "avi" or fileType == "mov")
				openVideoFile(filePath);
		}
		break;

	case QEvent::DragEnter:
		event->accept();

	default:
		break;
	}
	return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	// Replace the bool with the state of the button.
	if(ui->actionSave->isEnabled())
	{
		QString msg = "Joker is about to quit, would you save the session to a Strip file ?";
		QMessageBox box(QMessageBox::Question, "", msg, QMessageBox::Save | QMessageBox::No | QMessageBox::Cancel);
		box.setDefaultButton(QMessageBox::Save);
		int result = box.exec();
		if(result == QMessageBox::Save)
		{
			if(saveStrip())
				event->accept();
			else
				event->ignore();
		}
		else if(result == QMessageBox::Cancel)
			event->ignore();
		else
			event->accept();
	}
	else
		event->accept();

}

bool MainWindow::saveStrip()
{
	hideMediaPanel();
	QString savedFile = QFileDialog::getSaveFileName(this, "Save...", _settings->value("lastFolder", QDir::homePath()).toString(),"*.strip");

	if(savedFile != "")
	{
		QFile file(savedFile);

			// open a file
			if (!file.open(QIODevice::WriteOnly))
			{
				// show error message if not able to open file
				QMessageBox::warning(0, "Read only", "The file is in read only mode");
			}
			else
			{
				//if file is successfully opened then create XML
				QXmlStreamWriter* xmlWriter = new QXmlStreamWriter();
				// set device (here file)to streamwriter
				xmlWriter->setDevice(&file);
				// Writes a document start with the XML version number version.

				// Positive numbers indicate spaces, negative numbers tabs.
				xmlWriter->setAutoFormattingIndent(-1);
				xmlWriter->setAutoFormatting(true);

				// Indent is just for keeping in mind XML structure
				xmlWriter->writeStartDocument();
				xmlWriter->writeStartElement("strip");
					xmlWriter->writeStartElement("meta");

						xmlWriter->writeStartElement("generator");
						xmlWriter->writeAttribute("name", "Joker");
						xmlWriter->writeAttribute("version", APP_VERSION);
						xmlWriter->writeEndElement();

						xmlWriter->writeStartElement("media");
						xmlWriter->writeAttribute("type", "detx");
						xmlWriter->writeCharacters(_doc->getFilePath());
						xmlWriter->writeEndElement();

						xmlWriter->writeStartElement("media");
						xmlWriter->writeAttribute("type", "video");
						xmlWriter->writeAttribute("tcStamp", PhTimeCode::stringFromFrame(_videoEngine->frameStamp(), PhTimeCodeType25));
						xmlWriter->writeCharacters(_videoEngine->fileName());
						xmlWriter->writeEndElement();

						xmlWriter->writeStartElement("state");
						xmlWriter->writeAttribute("lastTimeCode", _strip->clock()->timeCode());
						xmlWriter->writeEndElement();

					xmlWriter->writeEndElement();
				xmlWriter->writeEndElement();

				xmlWriter->writeEndDocument();
				delete xmlWriter;
			}

		PHDEBUG << savedFile;

		return true;
	}
	return false;

}

void MainWindow::on_actionOpen_triggered()
{
	hideMediaPanel();

	QFileDialog dlg(this, "Open...", "", "Rythmo files (*.detx *.strip)");
	dlg.setFileMode(QFileDialog::ExistingFile);
	if(dlg.exec())
	{
		QString fileName = dlg.selectedFiles()[0];
		openFile(fileName);
	}

	fadeInMediaPanel();
}

void MainWindow::on_actionPlay_pause_triggered()
{
	if(_strip->clock()->rate() == 0.0)
		_strip->clock()->setRate(1.0);
	else
		_strip->clock()->setRate(0.0);
}

void MainWindow::on_actionPlay_backward_triggered()
{
	_strip->clock()->setRate(-1.0);
}

void MainWindow::on_actionStep_forward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setFrame(_strip->clock()->frame() + 1);
}

void MainWindow::on_actionStep_backward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setFrame(_strip->clock()->frame() - 1);
}

void MainWindow::on_actionStep_time_forward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setTime(_strip->clock()->time() + 1);
}

void MainWindow::on_actionStep_time_backward_triggered()
{
	_strip->clock()->setRate(0.0);
	_strip->clock()->setTime(_strip->clock()->time() - 1);
}

void MainWindow::on_action_3_triggered()
{
	_strip->clock()->setRate(-3.0);
}

void MainWindow::on_action_1_triggered()
{
	_strip->clock()->setRate(-1.0);
}

void MainWindow::on_action_0_5_triggered()
{
	_strip->clock()->setRate(-0.5);
}

void MainWindow::on_action0_triggered()
{
	_strip->clock()->setRate(0.0);
}

void MainWindow::on_action0_5_triggered()
{
	_strip->clock()->setRate(0.5);
}

void MainWindow::on_action1_triggered()
{
	_strip->clock()->setRate(1.0);
}

void MainWindow::on_action3_triggered()
{
	_strip->clock()->setRate(3.0);
}

void MainWindow::on_actionOpen_Video_triggered()
{
	hideMediaPanel();

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Movie"),QDir::homePath());
	if(openVideoFile(fileName))
		on_actionChange_timestamp_triggered();
	else
		QMessageBox::critical(this, "Error", "Unable to open " + fileName);

	fadeInMediaPanel();
}

bool MainWindow::openVideoFile(QString videoFileName)
{

	// Opening the corresponding video file if it exists
	QFileInfo fileInfo(videoFileName);
	if (fileInfo.exists())
	{
		_videoEngine->open(_doc->getVideoPath());
		_videoEngine->setFrameStamp(_doc->getVideoTimestamp());
		_mediaPanel.setFirstFrame(_doc->getVideoTimestamp());
		_mediaPanel.setMediaLength(_videoEngine->length());
		_sonySlave.clock()->setFrame(_doc->getVideoTimestamp());
	}
	else
	{
		QString msg = "Joker is unable to find a video file corresponding to your script file\"";
		msg +=  _doc->getVideoPath();
		msg +=  " \", would you do it manually?";
		QMessageBox box(QMessageBox::Question, "Open a video file ?", msg, QMessageBox::Yes | QMessageBox::No);
		box.setDefaultButton(QMessageBox::Yes);
		if(box.exec() == QMessageBox::Yes)
		{
			QString lastFolder = _settings->value("lastFolder", QDir::homePath()).toString();
			QFileDialog dlg(this, "Open...", lastFolder, "Movie files (*.avi *.mov)");
			if(dlg.exec())
			{
				_settings->setValue("lastFolder", dlg.directory().absolutePath());
				QString videoFile = dlg.selectedFiles()[0];
				_videoEngine->open(videoFile);
				_videoEngine->setFrameStamp(_doc->getVideoTimestamp());
				_mediaPanel.setFirstFrame(_doc->getVideoTimestamp());
				_mediaPanel.setMediaLength(_videoEngine->length());
				_sonySlave.clock()->setFrame(_doc->getVideoTimestamp());
			}
		}
	}
}

void MainWindow::on_actionChange_timestamp_triggered()
{
	hideMediaPanel();

	PhTimeCodeDialog dlg(_strip->clock()->timeCodeType(), _synchronizer.videoClock()->frame());
	if(dlg.exec() == QDialog::Accepted)
	{
		PhFrame frameStamp = _videoEngine->frameStamp();
		frameStamp += dlg.frame() - _synchronizer.videoClock()->frame();
		_videoEngine->setFrameStamp(frameStamp);
		_strip->clock()->setFrame(dlg.frame());
	}

	fadeInMediaPanel();
}



void MainWindow::on_actionAbout_triggered()
{
	hideMediaPanel();

	AboutMenu menu;
	menu.exec();

	fadeInMediaPanel();
}


void MainWindow::on_actionPreferences_triggered()
{
	hideMediaPanel();

	PreferencesDialog dlg(_settings);
	dlg.exec();
	fadeInMediaPanel();
}

void MainWindow::fadeInMediaPanel()
{
	_mediaPanel.show();
	_mediaPanelAnimation.stop();
	_mediaPanelAnimation.setDuration(300);
	_mediaPanelAnimation.setEndValue(1);
	_mediaPanelAnimation.setEasingCurve(QEasingCurve::InOutSine);
	_mediaPanelAnimation.start();
	_mediaPanelState = MediaPanelVisible;
	_mediaPanelTimer.start(3000);
}

void MainWindow::fadeOutMediaPanel()
{
	PHDEBUG << _mediaPanelState;
	switch(_mediaPanelState)
	{
	case MediaPanelVisible:
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
	}
}

void MainWindow::hideMediaPanel()
{
	_mediaPanel.hide();
	_mediaPanelState = MediaPanelHidden;
	_mediaPanelTimer.stop();
}


void MainWindow::on_actionProperties_triggered()
{
	_propertyDialog.show();
}

void MainWindow::on_actionTest_mode_triggered()
{
	if(_settings->value("stripTestMode", false).toBool())
		_settings->setValue("stripTestMode", false);
	else
		_settings->setValue("stripTestMode", true);
}

void MainWindow::on_actionTimecode_triggered()
{
	hideMediaPanel();

	PhTimeCodeDialog dlg(_strip->clock()->timeCodeType(), _strip->clock()->frame());
	if(dlg.exec() == QDialog::Accepted)
		_strip->clock()->setFrame(dlg.frame());

	fadeInMediaPanel();
}

void MainWindow::on_actionNext_element_triggered()
{
	PhFrame frame = _doc->getNextElementFrame(_strip->clock()->frame());
	if(frame < PHFRAMEMAX)
		_strip->clock()->setFrame(frame);
}

void MainWindow::on_actionPrevious_element_triggered()
{
	PhFrame frame = _doc->getPreviousElementFrame(_strip->clock()->frame());
	if(frame > PHFRAMEMIN)
		_strip->clock()->setFrame(frame);
}

void MainWindow::on_actionClear_list_triggered()
{
	//Open the recent group
	_settings->beginGroup("openRecent");
	//List all keys
	QStringList indexes = _settings->allKeys();
	//Remove them from
	foreach(QString index, indexes)
		_settings->remove(index);

	//Close the group
	_settings->endGroup();

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
	_recentFileButtons.clear();
	ui->menuOpen_recent->setEnabled(false);
}

void MainWindow::on_actionImport_triggered()
{
	hideMediaPanel();

	QString lastFolder = _settings->value("lastFolder", QDir::homePath()).toString();

	QString rythmoFileName;
	QFileDialog dlg(this, "Open...", lastFolder, "Rythmo files (*.detx)");
	if(dlg.exec())
	{
		_settings->setValue("lastFolder", dlg.directory().absolutePath());
		rythmoFileName = dlg.selectedFiles()[0];
		openFile(rythmoFileName);
		ui->actionSave->setEnabled(true);
	}

	fadeInMediaPanel();
}

void MainWindow::on_actionSave_triggered()
{
	saveStrip();
}
