#ifndef SONYTOOLWINDOW_H
#define SONYTOOLWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "PhSync/PhClock.h"
#include "PhSony/PhSonyMasterController.h"
#include "PhSony/PhSonySlaveController.h"

#include "SonyToolSettings.h"

namespace Ui {
class SonyToolWindow;
}

/**
 * @brief The SonyToolWindow class
 *
 * The status active/inactive of the ports are saved to the settings whenever they are changed
 * and loaded when the application is launched
 */
class SonyToolWindow : public QMainWindow
{
	Q_OBJECT

public:
	/**
	 * @brief The SonyToolWindow constructor
	 */
	explicit SonyToolWindow();

	~SonyToolWindow();

private slots:
	void masterNextFrame();
	void masterPreviousFrame();

	void onDeviceIdData(unsigned char id1, unsigned char id2);
	void onStatusData(unsigned char * statusData, int offset, int length);

	void on_masterActiveCheck_clicked(bool checked);
	void on_slaveActiveCheck_clicked(bool checked);

	void on_actionMaster_GoTo_triggered();

	void on_actionSlave_GoTo_triggered();

	void on_actionSlave_Use_video_sync_triggered(bool useVideo);

	void on_actionMaster_Use_video_sync_triggered(bool useVideo);

	void on_actionPreferences_triggered();

private:
	SonyToolSettings _settings;
	Ui::SonyToolWindow *ui;
	PhSonyMasterController _sonyMaster;
	PhSonySlaveController _sonySlave;
	QTimer _masterTimer, _slaveTimer;
};

#endif // SONYTOOLWINDOW_H
