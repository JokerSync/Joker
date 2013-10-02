#include "PhFFMpegVideoView.h"


PhFFMpegVideoView::PhFFMpegVideoView(QWidget *parent) :
	PhGraphicView(parent), _formatContext(NULL)
{
	qDebug() << "Using FFMpeg widget for video playback.";
	av_register_all();

	connect(&_clock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
	connect(&_clock, SIGNAL(rateChanged(PhRate)), this, SLOT(onRateChanged(PhRate)));
}

bool PhFFMpegVideoView::open(QString fileName)
{
	if(avformat_open_input(&_formatContext, fileName.toStdString().c_str(), NULL, NULL) == 0)
	{
		av_dump_format(_formatContext, 0, fileName.toStdString().c_str(), 0);
	}
	return false;
}

PhFFMpegVideoView::~PhFFMpegVideoView()
{
	if(_formatContext != NULL)
		avformat_close_input(&_formatContext);
}

bool PhFFMpegVideoView::init()
{

}

void PhFFMpegVideoView::paint()
{

}

void PhFFMpegVideoView::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
}

void PhFFMpegVideoView::onRateChanged(PhRate rate)
{
}

void PhFFMpegVideoView::checkVideoPosition()
{

}
