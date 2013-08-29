#ifndef PHVIDEOVIEW_H
#define PHVIDEOVIEW_H

#define USE_VLC

#ifdef USE_VLC
#define PhVideoView PhVLCVideoView
#include "PhVLCVideoView.h"
#else
#define PhVideoView PhQTVideoView
#include "PhQTVideoView.h"
#endif

#endif // PHVIDEOVIEW_H
