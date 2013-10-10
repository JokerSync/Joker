#include "PhFFMpegGLVideoView.h"

PhFFMpegGLVideoView::PhFFMpegGLVideoView(QWidget *parent) :
	PhGraphicView(parent)
{
	connect(&_clock, SIGNAL(frameChanged(PhFrame,PhTimeCodeType)), this, SLOT(onFrameChanged(PhFrame,PhTimeCodeType)));
}
