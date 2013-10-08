#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <PhGraphicStrip/PhGraphicStripView.h>
#include <QMessageBox>
#include <QSettings>
#include <StripPropertiesDialog.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void openFile(QString fileName);
	void createFile(int nbPeople, int nbLoop, int nbText, int nbTrack);


private slots:
	void onOpenFile();
	void onFrameChanged(PhFrame frame, PhTimeCodeType tcType);
	void onRateChanged(PhRate rate);

	void on_actionPlay_pause_triggered();

	void on_actionPlay_backward_triggered();

	void on_actionStep_forward_triggered();

	void on_actionStep_backward_triggered();

	void on_actionStep_time_forward_triggered();

	void on_actionStep_time_backward_triggered();

	void on_actionPrevious_Element_triggered();

	void on_actionNext_Element_triggered();

	void on_action_3_triggered();

	void on_action_1_triggered();

	void on_action_0_5_triggered();

	void on_action0_triggered();

	void on_action0_5_triggered();

	void on_action1_triggered();

	void on_action3_triggered();

	void on_actionGo_to_triggered();

	void on_actionFull_Screen_triggered();

	void on_actionStrip_Properties_triggered();

	void on_actionChange_font_triggered();

private:
	Ui::MainWindow *ui;
	QSettings _settings;
	PhGraphicStripView *_stripView;
	PhStripDoc *_doc;
	PhClock *_clock;
	StripPropertiesDialog *dlg;
	QString _path;
};

#endif // MAINWINDOW_H
