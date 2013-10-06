#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>

extern "C" {
#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	bool openFile(QString fileName);

protected:
	void paintEvent(QPaintEvent *);

private slots:
	void on_actionOpen_triggered();

	void on_actionPlay_pause_triggered();

	void on_actionNext_frame_triggered();

	void on_actionPrevious_frame_triggered();

private:
	Ui::MainWindow *ui;

	AVFormatContext * _pFormatContext;
	struct SwsContext * _pSwsCtx;
	QImage * _image;
	uint8_t *_rgb;
};

#endif // MAINWINDOW_H
