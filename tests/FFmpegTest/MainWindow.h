#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QTimer>

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

	bool setFrame(int frame);

protected:
	void paintEvent(QPaintEvent *);

private slots:
	void on_actionOpen_triggered();

	void on_actionPlay_pause_triggered();

	void on_actionNext_frame_triggered();

	void on_actionPrevious_frame_triggered();

	void onTimeOut();

private:
	Ui::MainWindow *ui;

	AVFormatContext * _pFormatContext;
	int _videoStream;
	AVCodecContext * _pCodecContext;
	AVFrame * _pFrame;
	struct SwsContext * _pSwsCtx;
	QImage * _image;
	uint8_t *_rgb;

	QTimer _timer;
	int _currentFrame;
};

#endif // MAINWINDOW_H
