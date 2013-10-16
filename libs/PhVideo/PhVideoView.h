#ifndef PHVIDEOVIEW_H
#define PHVIDEOVIEW_H

#ifdef USE_VLC
#define PhVideoView PhVLCVideoView
#include "PhVLCVideoView.h"
#endif
#ifdef USE_QTVIDEO
#define PhVideoView PhQTVideoView
#include "PhQTVideoView.h"
#endif
#ifdef USE_QTAV
#define PhVideoView PhQtAVVideoView
#include "PhQtAVVideoView.h"
#endif
#ifdef USE_FFMPEG_WIDGET
#define PhVideoView PhFFMpegWidgetVideoView
#include "PhFFMpegWidgetVideoView.h"
#endif
#ifdef USE_FFMPEG_GL
#define PhVideoView PhFFMpegGLVideoView
#include "PhFFMpegGLVideoView.h"
#endif

#endif // PHVIDEOVIEW_H
