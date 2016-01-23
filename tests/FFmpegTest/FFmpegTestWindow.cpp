#include "FFmpegTestWindow.h"
#include "ui_FFmpegTestWindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QPainter>

FFmpegTestWindow::FFmpegTestWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::FFmpegTestWindow),
	_pFormatContext(NULL),
	_videoStream(NULL),
	_pCodecContext(NULL),
	_pFrame(NULL),
	_pSwsCtx(NULL),
	_image(NULL),
	_rgb(NULL),
	_currentFrame(0),
	_rate(0)
{
	ui->setupUi(this);

	av_register_all();

	connect(&_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
	_timer.start(40);
}

FFmpegTestWindow::~FFmpegTestWindow()
{
	delete ui;
}

bool FFmpegTestWindow::openFile(QString fileName)
{
	qDebug() << fileName;
	if(avformat_open_input(&_pFormatContext, fileName.toStdString().c_str(), NULL, NULL) < 0)
		return false;

	// Retrieve stream information
	if (avformat_find_stream_info(_pFormatContext, NULL) < 0)
		return -1; // Couldn't find stream information

	av_dump_format(_pFormatContext, 0, fileName.toStdString().c_str(), 0);

	// Find video stream :
	for(unsigned int i = 0; i < _pFormatContext->nb_streams; i++) {
		if(_pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
			_videoStream = _pFormatContext->streams[i];
			break;
		}
	}

	if(_videoStream == NULL)
		return false;

	_pCodecContext = _videoStream->codec;

	qDebug() << "size : " << _pCodecContext->width << "x" << _pCodecContext->height;
	AVCodec * pCodec = avcodec_find_decoder(_pCodecContext->codec_id);
	if(pCodec == NULL) {
		qDebug() << "Unable to find decoder";
		return false;
	}

	if(avcodec_open2(_pCodecContext, pCodec, NULL) < 0) {
		qDebug() << "Unable to open decoder";
		return false;
	}

	_pFrame = av_frame_alloc();

	resizeEvent(NULL);

	_currentFrame = 0;
	return setFrame(0);
}

bool FFmpegTestWindow::setFrame(int frame)
{
	if(!_videoStream)
		return false;
	av_seek_frame(_pFormatContext, _videoStream->index, frame, 0);

	AVPacket packet;
	while(av_read_frame(_pFormatContext, &packet) >= 0) {
		if(packet.stream_index == _videoStream->index) {
			int ok;
			avcodec_decode_video2(_pCodecContext, _pFrame, &ok, &packet);
			if(!ok)
				return false;

			int linesize = _image->width() * 3;
			// Convert the image into YUV format that SDL uses
			if (sws_scale(_pSwsCtx, (const uint8_t * const *) _pFrame->data,
			              _pFrame->linesize, 0, _pCodecContext->height, &_rgb,
			              &linesize) < 0)
				return false;
			return true;
		}
	}
	return false;
}

void FFmpegTestWindow::paintEvent(QPaintEvent *)
{
	if(_image) {
		QPainter painter(this);
		painter.drawImage(0, 0, *_image);
	}
}

void FFmpegTestWindow::resizeEvent(QResizeEvent *)
{
	if(_pCodecContext == NULL)
		return;
	int w = this->width();
	int h = this->height();

	// adjust width to a multiple of 4:
	int pow = 4;
	if(w % pow)
		w += pow - (w % pow);

	if(_rgb)
		delete _rgb;
	_rgb = new uint8_t[3 * w * h];

	if(_image)
		delete _image;

	_image = new QImage(_rgb, w, h, QImage::Format_RGB888);


	_pSwsCtx = sws_getContext(_pCodecContext->width, _pCodecContext->height,
	                          _pCodecContext->pix_fmt, w, h,
	                          AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL);
}

void FFmpegTestWindow::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open...");
	if(QFile::exists(fileName)) {
		if(!openFile(fileName))
			QMessageBox::critical(this, "Error", "Unable to open " + fileName);
	}
}

void FFmpegTestWindow::on_actionPlay_pause_triggered()
{
	if(_rate)
		_rate = 0;
	else
		_rate = 1;
}

void FFmpegTestWindow::on_actionNext_frame_triggered()
{
	this->setFrame(++_currentFrame);
}

void FFmpegTestWindow::on_actionPrevious_frame_triggered()
{
	this->setFrame(--_currentFrame);
}

void FFmpegTestWindow::onTimeOut()
{
	_currentFrame += _rate;
	this->setFrame(_currentFrame);
	this->update();
}
