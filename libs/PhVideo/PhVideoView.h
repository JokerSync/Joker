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
#ifdef USE_FFMPEG
#define PhVideoView PhFFMpegVideoView
#include "PhFFMpegVideoView.h"
#endif

#endif // PHVIDEOVIEW_H
