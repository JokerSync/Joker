/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QCoreApplication>
#include <QThread>

#include "PhTools/PhDebug.h"

#include "PhPlanarVideoBuffer.h"
#include "PhVideoDecoder.h"

PhVideoDecoder::PhVideoDecoder(PhVideoSettings *settings) :
	_fileName(""),
	_tcType(PhTimeCodeType25),
	_formatContext(NULL),
	_videoStream(NULL),
	_videoFrame(NULL),
	_swsContext(NULL),
	_currentFrame(PHFRAMEMIN),
	_stripFrame(PHFRAMEMIN),
	_seekFrame(PHFRAMEMIN),
	_useAudio(false),
	_audioStream(NULL),
	_audioFrame(NULL),
	_deinterlace(false),
	_backward(false),
	_processing(false),
	_seek(false),
	_fastSeek(false),
	_readAheadCount(settings->videoReadhead()),
	_maxAllocatedCount(settings->videoPoolSize() + settings->videoReadhead()),
	_allocatedCount(0),
	_seekThreshold(settings->videoReadhead())
{
	PHDEBUG << "Using FFMpeg widget for video playback.";
	av_register_all();
	avcodec_register_all();

	// register PhVideoBuffer* so that it can be used in signals/slots
	qRegisterMetaType<PhVideoBuffer*>();
}

bool PhVideoDecoder::ready()
{
	return (_formatContext && _videoStream && _videoFrame);
}

void PhVideoDecoder::open(QString fileName)
{
	close();
	PHDEBUG << fileName;

	_currentFrame = PHFRAMEMIN;
	_seekFrame = PHFRAMEMIN;

	int openErrorCode = avformat_open_input(&_formatContext, fileName.toStdString().c_str(), NULL, NULL);
	if(openErrorCode < 0) {
		char errorString[AV_ERROR_MAX_STRING_SIZE];
		av_strerror(openErrorCode, errorString, AV_ERROR_MAX_STRING_SIZE);
		PHDEBUG << errorString;
		emit openFailed();
		close();
		return;
	}

	PHDEBUG << "Retrieve stream information";
	if (avformat_find_stream_info(_formatContext, NULL) < 0) {
		emit openFailed();
		close();
		return; // Couldn't find stream information
	}

	// Disable dump for specs
	if(PhDebug::logMask() & 1)
		av_dump_format(_formatContext, 0, fileName.toStdString().c_str(), 0);

	// Find video stream :
	for(int i = 0; i < (int)_formatContext->nb_streams; i++) {
		AVMediaType streamType = _formatContext->streams[i]->codec->codec_type;
		PHDEBUG << i << ":" << streamType;
		switch(streamType) {
		case AVMEDIA_TYPE_VIDEO:
			// Some containers are advertised with several video streams.
			// For example, one is the main stream and the other one is just a cover picture (single frame).
			// Here we choose the one that has the largest number of frames.
			if (!_videoStream || _videoStream->nb_frames < _formatContext->streams[i]->nb_frames) {
				_videoStream = _formatContext->streams[i];
			}
			PHDEBUG << "\t=> video";
			break;
		case AVMEDIA_TYPE_AUDIO:
			if(_useAudio && (_audioStream == NULL))
				_audioStream = _formatContext->streams[i];
			PHDEBUG << "\t=> audio";
			break;
		default:
			PHDEBUG << "\t=> unknown";
			break;
		}
	}

	if(_videoStream == NULL) {
		emit openFailed();
		close();
		return;
	}

	// Looking for timecode type
	_tcType = PhTimeCode::computeTimeCodeType(this->framePerSecond());

	PHDEBUG << "size : " << _videoStream->codec->width << "x" << _videoStream->codec->height;
	AVCodec * videoCodec = avcodec_find_decoder(_videoStream->codec->codec_id);
	if(videoCodec == NULL) {
		PHDEBUG << "Unable to find the codec:" << _videoStream->codec->codec_id;
		emit openFailed();
		close();
		return;
	}

	const AVCodecDescriptor *videoCodecDescriptor = avcodec_descriptor_get(videoCodec->id);
	if (videoCodecDescriptor
	    && videoCodecDescriptor->type == AVMEDIA_TYPE_VIDEO
	    && (videoCodecDescriptor->props & AV_CODEC_PROP_INTRA_ONLY)) {
		// Seeking is efficient for codecs which have only intra-frame compression.
		// With these codecs, all frames are key-frames.
		// This includes MJPEG.
		// In that case, choose a very low seek threshold.
		_seekThreshold = 1;
	}
	else {
		_seekThreshold = _readAheadCount;
	}

	if (avcodec_open2(_videoStream->codec, videoCodec, NULL) < 0) {
		PHDEBUG << "Unable to open the codec:" << _videoStream->codec;
		emit openFailed();
		close();
		return;
	}

	_videoFrame = av_frame_alloc();

	if(_audioStream) {
		AVCodec* audioCodec = avcodec_find_decoder(_audioStream->codec->codec_id);
		if(audioCodec) {
			if(avcodec_open2(_audioStream->codec, audioCodec, NULL) < 0) {
				PHDEBUG << "Unable to open audio codec.";
				_audioStream = NULL;
			}
			else {
				_audioFrame = av_frame_alloc();
				PHDEBUG << "Audio OK.";
			}
		}
		else {
			PHDEBUG << "Unable to find codec for audio.";
			_audioStream = NULL;
		}
	}

	_fileName = fileName;

	emit opened(_tcType, frameIn(), frameLength(), width(), height(), codecName());
}

void PhVideoDecoder::close()
{
	PHDEBUG << _fileName;

	if (_swsContext) {
		sws_freeContext(_swsContext);
		_swsContext = NULL;
	}

	if(_formatContext) {
		PHDEBUG << "Close the media context.";
		if(_videoStream)
			avcodec_close(_videoStream->codec);
		if(_audioStream)
			avcodec_close(_audioStream->codec);
		avformat_close_input(&_formatContext);
	}

	if (_videoFrame) {
		av_frame_free(&_videoFrame);
		_videoFrame = NULL;
	}

	_formatContext = NULL;
	_videoStream = NULL;
	_audioStream = NULL;
	PHDEBUG << _fileName << "closed";

	_fileName = "";

	foreach(PhVideoBuffer * frame, _recycledFrames) {
		delete frame;
	}
	_recycledFrames.clear();

	_allocatedCount = 0;
}

PhFrame PhVideoDecoder::clip(PhFrame frame)
{
	// clip to video boundaries
	if (frame >= this->frameLength())
		frame = this->frameLength() - 1;
	if(frame < 0)
		frame = 0;

	return frame;
}

void PhVideoDecoder::stripTimeChanged(PhFrame stripFrame, bool backward, bool stripFrameIsInPool)
{
	_backward = backward;

	stripFrame = clip(stripFrame);

	bool backwardChange = stripFrame < _currentFrame;
	bool largeForwardChange =
		stripFrame > _currentFrame + _seekThreshold
		&& stripFrame > _stripFrame + _seekThreshold;

	if (!stripFrameIsInPool) {
		if (largeForwardChange) {
			// scenario 1: seeking forward (independently of playing direction)
			PHDEBUG << "fast forward seek";
			if (!backward) {
				// when playing forward, go faster by not decoding in-between frames
				_fastSeek = true;
			}
			else {
				_seek = true;
			}

			_seekFrame = stripFrame;
		}
		else if (!backward && backwardChange) {
			// scenario 2: seeking backward while paused or playing forward
			PHDEBUG << "forward seek to the past";
			_seek = true;
			_seekFrame = stripFrame;
		}
		else if (backward && backwardChange) {
			// scenario 3: playing backward and stripFrame is in the past compared to decoder position
			PHDEBUG << "backward seek to the past";
			_seek = true;
			// the trick is here:
			// it is necessary to go further back in time
			// because the decoder can only decode forward
			_seekFrame = clip(stripFrame - _readAheadCount);
		}
	}
	else {
		if (backward
		    && backwardChange
		    && stripFrame >= _seekFrame
		    && stripFrame < _seekFrame + _readAheadCount) {
			// scenario 4: similar to scenario 3 but anticipated
			PHDEBUG << "backward seek to the past - anticipated";
			_seek = true;
			// the trick is here:
			// it is necessary to go further back in time
			// because the decoder can only decode forward
			_seekFrame = clip(stripFrame - _readAheadCount);
		}
	}

	_stripFrame = stripFrame;

	if (!_processing) {
		_processing = true;

		// process events recursively
		while (canDecode()) {
			QCoreApplication::processEvents();
			decodeFrame();
		}

		_processing = false;
	}
}

void PhVideoDecoder::stop()
{
	_stripFrame = PHFRAMEMIN;
}

bool PhVideoDecoder::canDecode()
{
	if (!ready()) {
		return false;
	}

	if (_stripFrame == PHFRAMEMIN) {
		return false;
	}

	if (!hasAvailableFrame()) {
		PHDBG(24) << "no frame buffer available";
		return false;
	}

	if (_seek || (_currentFrame == PHFRAMEMIN)) {
		return true;
	}

	return readAheadFrame() > _currentFrame + 1;
}

PhFrame PhVideoDecoder::readAheadFrame()
{
	return clip(_stripFrame + _readAheadCount);
}

void PhVideoDecoder::setDeinterlace(bool deinterlace)
{
	PHDEBUG << deinterlace;

	if (deinterlace != _deinterlace) {
		_deinterlace = deinterlace;
	}
}

PhVideoDecoder::~PhVideoDecoder()
{
	close();
}

PhFrame PhVideoDecoder::frameLength()
{
	// Note: computing the frame length from the format context's duration seems to give bogus results.
	// Use simply the stream number of frames instead.
	if(_videoStream)
		return _videoStream->nb_frames;
	return 0;
}

double PhVideoDecoder::framePerSecond()
{
	// default is 25 fps.
	// It will be used when loading a collection of image files (as it is done in the tests and specs),
	// where ffmpeg framerate is undefined (avg_frame_rate.den is 0).
	double result = 25.00f;

	if(_videoStream && (_videoStream->avg_frame_rate.den != 0)) {
		result =  av_q2d(_videoStream->avg_frame_rate);
	}

	return result;
}

void PhVideoDecoder::emitFrame(AVFrame *avFrame)
{
	PhVideoBuffer *buffer = getAvailableFrame();
	buffer->setAvFrame(avFrame);
	buffer->setFrame(_currentFrame);

	// tell the video engine that we have finished decoding!
	emit frameAvailable(buffer);
}

void PhVideoDecoder::decodeFrame()
{
	if(!ready()) {
		PHDBG(24) << "not ready";
		return;
	}

	if (!canDecode()) {
		// readahead position is already reached
		return;
	}

	PhFrame frame;
	if(_seek || _fastSeek) {
		frame = _seekFrame;

		// seek to the closest keyframe in the past
		int flags = AVSEEK_FLAG_BACKWARD;
		int64_t timestamp = PhFrame_to_AVTimestamp(_seekFrame);
		PHDEBUG << "seek:" << frame << _seekFrame << _currentFrame << " " << frame - _currentFrame << " " << timestamp;
		av_seek_frame(_formatContext, _videoStream->index, timestamp, flags);

		avcodec_flush_buffers(_videoStream->codec);
	}
	else {
		// just go on with the next frame in the stream
		frame = _currentFrame + 1;
	}

	// fast-forward to the desired frame
	// without fully decoding all in-between frames
	if (_fastSeek) {
		fastForwardToFrame(_seekFrame);
		return;
	}

	AVPacket packet;

	int frameFinished = 0;
	while (!frameFinished) {
		int error = av_read_frame(_formatContext, &packet);
		if (error == 0) {
			if (packet.stream_index == _videoStream->index) {

				avcodec_decode_video2(_videoStream->codec, _videoFrame, &frameFinished, &packet);
				if(frameFinished) {
					// update the current position of the engine
					// (Note that it is best not to do use '_currentTime = time' here, because the seeking operation may
					// not be 100% accurate: the actual time may be different from the requested time. So a time drift
					// could appear.)
					_currentFrame = AVTimestamp_to_PhFrame(av_frame_get_best_effort_timestamp(_videoFrame));

					PHDBG(24) << frame << _currentFrame;
					emitFrame(_videoFrame);

					if (_seek) {
						_seek = false;
						_seekFrame = _currentFrame;
					}

				} // if frame decode is not finished, let's read another packet.
			}
			else if(_audioStream && (packet.stream_index == _audioStream->index)) {
				int ok = 0;
				avcodec_decode_audio4(_audioStream->codec, _audioFrame, &ok, &packet);
				if(ok) {
					PHDBG(24) << "audio:" << _audioFrame->nb_samples;
				}
			}
		}
		else {
			char errorStr[256];
			av_strerror(error, errorStr, 256);
			PHDBG(24) << frame << "error:" << errorStr;
			frameFinished = true;
		}

		//Avoid memory leak
		av_free_packet(&packet);
	}
}

void PhVideoDecoder::fastForwardToFrame(PhFrame frame)
{
	AVPacket packet;

	// let the loop start even if seeking backwards in time
	_currentFrame = PHFRAMEMIN;

	while (_currentFrame < frame) {
		int error = av_read_frame(_formatContext, &packet);
		int frameFinished = 0;
		if (error == 0) {
			if(packet.stream_index == _videoStream->index) {
				avcodec_decode_video2(_videoStream->codec, _videoFrame, &frameFinished, &packet);
				if(frameFinished) {
					// update the current position of the engine
					// (Note that it is best not to do use '_currentTime = time' here, because the seeking operation may
					// not be 100% accurate: the actual time may be different from the requested time. So a time drift
					// could appear.)
					_currentFrame = AVTimestamp_to_PhFrame(av_frame_get_best_effort_timestamp(_videoFrame));

					if (frame == _currentFrame) {
						emitFrame(_videoFrame);

						_fastSeek = false;
						_seekFrame = _currentFrame;
					}
				} // if frame decode is not finished, let's read another packet.
			}
			else if(_audioStream && (packet.stream_index == _audioStream->index)) {
				int ok = 0;
				avcodec_decode_audio4(_audioStream->codec, _audioFrame, &ok, &packet);
				if(ok) {
					PHDBG(24) << "audio:" << _audioFrame->nb_samples;
				}
			}
		}
		else {
			char errorStr[256];
			av_strerror(error, errorStr, 256);
			PHDBG(24) << frame << "error:" << errorStr;

			//Avoid memory leak
			av_free_packet(&packet);
			break;
		}

		//Avoid memory leak
		av_free_packet(&packet);
	}
}

void PhVideoDecoder::recycleFrame(PhVideoBuffer *frame)
{
	_recycledFrames.append(frame);
}

PhVideoBuffer* PhVideoDecoder::getAvailableFrame()
{
	PhVideoBuffer * buffer = NULL;

	if (_allocatedCount < _maxAllocatedCount) {
		PHDBG(24) << "creating a new PhVideoBuffer " << _allocatedCount + 1 << _maxAllocatedCount;
		QVideoFrame::PixelFormat q_pixel_format = toQVideoFramePixelFormat(_videoStream->codec->pix_fmt);
		buffer = new PhVideoBuffer(width(), height(), q_pixel_format, _videoStream->codec->pix_fmt);
		_allocatedCount += 1;
	} else if (!_recycledFrames.empty()) {
		PHDBG(24) << "recycling a PhVideoBuffer";
		buffer = _recycledFrames.takeFirst();
	}

	return buffer;
}

bool PhVideoDecoder::hasAvailableFrame()
{
	return !_recycledFrames.empty() || _allocatedCount < _maxAllocatedCount;
}

int PhVideoDecoder::width()
{
	if(_videoStream)
		return _videoStream->codec->width;
	return 0;
}

int PhVideoDecoder::height()
{
	if(_videoStream)
		return _videoStream->codec->height;
	return 0;
}

QString PhVideoDecoder::codecName()
{
	if(_videoStream)
		return _videoStream->codec->codec_descriptor->long_name;

	return "";
}

PhFrame PhVideoDecoder::frameIn()
{
	PhFrame frameIn = 0;

	AVDictionaryEntry *tag = av_dict_get(_formatContext->metadata, "timecode", NULL, AV_DICT_IGNORE_SUFFIX);
	if(tag == NULL)
		tag = av_dict_get(_videoStream->metadata, "timecode", NULL, AV_DICT_IGNORE_SUFFIX);

	if(tag) {
		PHDEBUG << "Found timestamp:" << tag->value;
		frameIn = PhTimeCode::frameFromString(tag->value, _tcType);
	}

	return frameIn;
}

int64_t PhVideoDecoder::PhFrame_to_AVTimestamp(PhFrame frame)
{
	int64_t timestamp = 0;
	if(_videoStream) {
		timestamp = frame * PhTimeCode::timePerFrame(_tcType) * _videoStream->time_base.den / _videoStream->time_base.num / PHTIMEBASE + _videoStream->start_time;
	}
	return timestamp;
}

PhFrame PhVideoDecoder::AVTimestamp_to_PhFrame(int64_t timestamp)
{
	PhFrame frame = 0;
	if(_videoStream)
		frame = (timestamp -_videoStream->start_time) * PHTIMEBASE * _videoStream->time_base.num / _videoStream->time_base.den / PhTimeCode::timePerFrame(_tcType);
	return frame;
}

QVideoFrame::PixelFormat PhVideoDecoder::toQVideoFramePixelFormat(enum AVPixelFormat pix_format)
{
	// the following values for QVideoFrame::PixelFormat have no corresponding AVPixelFormat:
	//	Format_ARGB8565_Premultiplied,
	//	Format_BGRA5658_Premultiplied,
	//	Format_AYUV444,
	//	Format_AYUV444_Premultiplied,
	//	Format_YUV444,
	//	Format_YV12
	//	Format_IMC1,
	//	Format_IMC2,
	//	Format_IMC3,
	//	Format_IMC4,
	//	Format_Jpeg,
	//	Format_CameraRaw,
	//	Format_AdobeDng
	switch (pix_format) {
	case AV_PIX_FMT_ARGB:
		return QVideoFrame::Format_ARGB32;
	case AV_PIX_FMT_BGRA:
		return QVideoFrame::Format_BGRA32;
	case AV_PIX_FMT_0RGB:
		return QVideoFrame::Format_RGB32;
	case AV_PIX_FMT_BGR0:
		return QVideoFrame::Format_BGR32;
	case AV_PIX_FMT_RGB24:
		return QVideoFrame::Format_RGB24;
	case AV_PIX_FMT_BGR24:
		return QVideoFrame::Format_BGR24;
	case AV_PIX_FMT_RGB565LE:
	case AV_PIX_FMT_RGB565BE:
		return QVideoFrame::Format_RGB565;
	case AV_PIX_FMT_BGR565LE:
	case AV_PIX_FMT_BGR565BE:
		return QVideoFrame::Format_BGR565;
	case AV_PIX_FMT_RGB555LE:
	case AV_PIX_FMT_RGB555BE:
		return QVideoFrame::Format_RGB555;
	case AV_PIX_FMT_BGR555LE:
	case AV_PIX_FMT_BGR555BE:
		return QVideoFrame::Format_BGR555;
	case AV_PIX_FMT_YUVJ420P:
	case AV_PIX_FMT_YUV420P:
		return QVideoFrame::Format_YUV420P;
	case AV_PIX_FMT_YUYV422:
		return QVideoFrame::Format_YUYV;
	case AV_PIX_FMT_UYVY422:
		return QVideoFrame::Format_UYVY;
	case AV_PIX_FMT_NV12:
		return QVideoFrame::Format_NV12;
	case AV_PIX_FMT_NV21:
		return QVideoFrame::Format_NV21;
	case AV_PIX_FMT_GRAY8:
		return QVideoFrame::Format_Y8;
	case AV_PIX_FMT_GRAY16:
		return QVideoFrame::Format_Y16;
	default:
		return QVideoFrame::Format_Invalid;
	}
}
