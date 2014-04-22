#ifndef FFMPEGTESTWINDOW_H
#define FFMPEGTESTWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QTimer>

extern "C" {
#ifndef INT64_C
/** see http://code.google.com/p/ffmpegsource/issues/detail?id=11#c13 */
#define INT64_C(c) (c ## LL)
/** and http://code.google.com/p/ffmpegsource/issues/detail?id=11#c23 */
#define UINT64_C(c) (c ## ULL)
#endif

#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

namespace Ui {
class FFmpegTestWindow;
}

class FFmpegTestWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit FFmpegTestWindow(QWidget *parent = 0);
	~FFmpegTestWindow();

	bool openFile(QString fileName);

	bool setFrame(int frame);

protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);

private slots:
	void on_actionOpen_triggered();

	void on_actionPlay_pause_triggered();

	void on_actionNext_frame_triggered();

	void on_actionPrevious_frame_triggered();

	void onTimeOut();

private:
	Ui::FFmpegTestWindow *ui;

	AVFormatContext * _pFormatContext;
	AVStream *_videoStream;
	AVCodecContext * _pCodecContext;
	AVFrame * _pFrame;
	struct SwsContext * _pSwsCtx;
	QImage * _image;
	uint8_t *_rgb;

	QTimer _timer;
	int _currentFrame;
	int _rate;
};

#endif // FFMPEGTESTWINDOW_H
