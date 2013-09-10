#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <PhGraphicStrip/PhGraphicStripView.h>

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

private slots:
	void onOpenFile();
	void onFrameOrRateChanged(PhFrame frame, PhTimeCodeType tcType);

	void on_actionPlay_pause_triggered();

	void on_actionPlay_backward_triggered();

	void on_actionStep_forward_triggered();

	void on_actionStep_backward_triggered();

private:
	Ui::MainWindow *ui;
	PhGraphicStripView *_stripView;
	PhStripDoc *_doc;
	PhClock *_clock;
};

#endif // MAINWINDOW_H
