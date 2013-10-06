#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	_pFormatContext(NULL),
	_image(NULL),
	_rgb(NULL)
{
	ui->setupUi(this);

	av_register_all();
}

MainWindow::~MainWindow()
{
	delete ui;
}

bool MainWindow::openFile(QString fileName)
{
	qDebug() << fileName;
	if(avformat_open_input(&_pFormatContext, fileName.toStdString().c_str(), NULL, NULL) < 0)
		return false;

	// Retrieve stream information
	if (avformat_find_stream_info(_pFormatContext, NULL) < 0)
		return -1; // Couldn't find stream information

	av_dump_format(_pFormatContext, 0, fileName.toStdString().c_str(), 0);

	int videoStream = -1;
	// Find video stream :
	for(int i = 0; i < _pFormatContext->nb_streams; i++)
	{
		if(_pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			break;
		}
	}

	if(videoStream == -1)
		return false;

	AVCodecContext * pCodecContext = _pFormatContext->streams[videoStream]->codec;

	qDebug() << "size : " << pCodecContext->width << "x" << pCodecContext->height;
	AVCodec * pCodec = avcodec_find_decoder(pCodecContext->codec_id);
	if(pCodec == NULL)
		return false;

	if(avcodec_open2(pCodecContext, pCodec, NULL) < 0)
		return false;

	_pFrame = avcodec_alloc_frame();

	struct SwsContext * pSwsCtx = sws_getContext(pCodecContext->width, pCodecContext->height,
							pCodecContext->pix_fmt, pCodecContext->width, pCodecContext->height,
							AV_PIX_FMT_RGB24, SWS_FAST_BILINEAR, NULL, NULL, NULL);

	AVPacket packet;
	while(av_read_frame(_pFormatContext, &packet) >= 0)
	{
		if(packet.stream_index == videoStream)
		{
			int ok;
			avcodec_decode_video2(pCodecContext, _pFrame, &ok, &packet);
			if(ok)
			{
				int linesize = 720;
				if(_rgb)
					delete _rgb;
				_rgb = new uint8_t[pCodecContext->width * pCodecContext->height * 3];
				// Convert the image into YUV format that SDL uses
				if(sws_scale(pSwsCtx, (const uint8_t * const *) _pFrame->data,
						  _pFrame->linesize, 0, pCodecContext->height, &_rgb,
						  &linesize) < 0)
					return false;

				if(_image)
					delete _image;

				_image = new QImage(_rgb, pCodecContext->width, pCodecContext->height, QImage::Format_RGB888);
			}
			break;
		}
	}
	return true;
}

void MainWindow::paintEvent(QPaintEvent *)
{
	if(_image)
	{
		QPainter painter(this);
		painter.drawImage(0, 0, *_image);
	}
}

void MainWindow::on_actionOpen_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open...");
	if(QFile::exists(fileName))
	{
		if(!openFile(fileName))
			QMessageBox::critical(this, "Error", "Unable to open " + fileName);
	}
}

void MainWindow::on_actionPlay_pause_triggered()
{
	// TODO
}

void MainWindow::on_actionNext_frame_triggered()
{
	// TODO
}

void MainWindow::on_actionPrevious_frame_triggered()
{
	// TODO
}
