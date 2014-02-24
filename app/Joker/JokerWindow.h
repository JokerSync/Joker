///
/// @file
/// @copyright (C) 2012-2014 Phonations
/// @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
///

#ifndef JOKERWINDOW_H
#define JOKERWINDOW_H

#include <QMessageBox>
#include <QPropertyAnimation>

#include "PhSync/PhSonySlaveController.h"
#include "PhCommonUI/PhFloatingMediaPanel.h"
#if USE_LTC
#include "PhSync/PhLtcReader.h"
#endif

#include "PhCommonUI/PhDocumentWindow.h"
#include "VideoStripView.h"
#include "Synchronizer.h"
#include "PropertyDialog.h"
#include "JokerSettings.h"

namespace Ui {
class JokerWindow;
}

///
/// @brief Joker main application window
///
/// The JokerWindow class implements the main screen user interface behaviour:
/// - Display the VideoStripView
/// - Handling PhFloatingMediaPanel behaviour
/// - Opening application dialog : preferences, open file, open video file, save , display properties, timestamp, people selection
/// - Display properties dialog
/// - Handling controls command
/// - Connect the application modules: PhVideoEngine, PhGraphicStrip, Synchronizer, PhSonySlaveController, PhLtcReader
///
class JokerWindow : public PhDocumentWindow
{
	Q_OBJECT

public:
	///
	/// @brief JokerWindow The JokerWindow constructor
	///
	/// @param settings The application settings
	///
	explicit JokerWindow(JokerSettings *settings);
	~JokerWindow();

	///
	/// @brief Open a video file
	///
	/// Open a videofile and set the framestamp to the videofile's value or the strip's value if the first one is not usable.
	///
	/// @param videoFile The videofile path
	///
	/// @return True if the videoFile opened well, false otherwise.
	///
	bool openVideoFile(QString videoFile);

protected:
	///
	/// @brief Open all supported strip file
	///
	/// @param filePath The file path
	///
	bool openDocument(QString filePath);

	///
	/// @brief Custom event filter
	///
	/// @param sender The event sender
	/// @param evenet The event
	/// @return True if handled, false otherwise
	///
	bool eventFilter(QObject *sender, QEvent *event);

	///
	/// @brief The PhFloatingMediaPanel state enumeration
	///
	/// The enumeration is used to handle the different state of the PhFloatingMediaPanel:
	/// visible, hidding and hidden for a best fade-in and fade-out effect.
	///
	enum MediaPanelState {
		MediaPanelVisible,
		MediaPanelHidding,
		MediaPanelHidden
	};

	///
	/// @brief Give the ui->menuOpen_recent item to PhDocumentWindow
	///
	/// PhDocumentWindow will fill the submenu item with the
	/// last document opened when calling setCurrentDocument().
	/// The max number of item is the PhDocumentWindowSettings::maxRecentDocument().
	///
	/// @return A reference to the menu item
	///
	QMenu *recentDocumentMenu();

	///
	/// @brief Give the ui->actionFullscreen to PhWindow
	///
	/// PhWindow will make it checkable and check and uncheck it
	/// each time the user toggle between fullscreen and normal.
	/// The action must be connected in JokerWindow constructor.
	/// @todo connect the action with PhWindow
	/// @return A reference to the action
	///
	QAction *fullScreenAction();

	///
	/// @brief Setup the synchronisation protocol
	///
	/// Close all the protocol if opened and setup the one specified
	/// by the settings.
	/// If it failed to open a protocol, go to the NoSync mode (and store
	/// it in the settings).
	/// The PhMediaPanel slider is enabled only in the NoSync mode.
	///
	void setupSyncProtocol();

	///
	/// @brief Event called when the user try to close the window.
	///
	/// @param event The event
	///
	void closeEvent(QCloseEvent *event);

	///
	/// @brief Check if the current document need to be save.
	///
	bool checkSaveFile();

private slots:
	// Qt Designer slots
	void on_actionOpen_triggered();

	void on_actionPlay_pause_triggered();

	void on_actionPlay_backward_triggered();

	void on_actionStep_forward_triggered();

	void on_actionStep_backward_triggered();

	void on_actionStep_time_forward_triggered();

	void on_actionStep_time_backward_triggered();

	void on_action_3_triggered();

	void on_action_1_triggered();

	void on_action_0_5_triggered();

	void on_action0_triggered();

	void on_action0_5_triggered();

	void on_action1_triggered();

	void on_action3_triggered();

	void on_actionOpen_Video_triggered();

	void on_actionChange_timestamp_triggered();

	void on_actionAbout_triggered();

	void on_actionPreferences_triggered();

	void on_actionClear_list_triggered();


	// Custom slots

	void fadeInMediaPanel();

	void fadeOutMediaPanel();

	void hideMediaPanel();

	void on_actionProperties_triggered();

	void on_actionTest_mode_triggered();

	void on_actionTimecode_triggered();

	void on_actionNext_element_triggered();

	void on_actionPrevious_element_triggered();

	void on_actionSave_triggered();

	void on_actionSave_as_triggered();

	void on_actionSelect_character_triggered();

	void on_actionForce_16_9_ratio_triggered();

private:
	Ui::JokerWindow *ui;
	PhGraphicStrip * _strip;
	PhVideoEngine * _videoEngine;
	PhStripDoc *_doc;
	JokerSettings *_settings;
	PhSonySlaveController _sonySlave;
	Synchronizer _synchronizer;

	PhFloatingMediaPanel _mediaPanel;
	QTimer _mediaPanelTimer;
	MediaPanelState _mediaPanelState;
	QPropertyAnimation _mediaPanelAnimation;

	PropertyDialog _propertyDialog;

#if USE_LTC
	PhLtcReader _ltcReader;
#endif
	bool _needToSave;
};

#endif // MAINWINDOW_H
