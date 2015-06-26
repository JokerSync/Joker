/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QCoreApplication>

#include "PhTools/PhDebug.h"

#include "PhVideoDecoder.h"

PhVideoDecoder::PhVideoDecoder() :
	_fileName(""),
	_tcType(PhTimeCodeType25),
	_formatContext(NULL),
	_videoStream(NULL),
	_videoFrame(NULL),
	_swsContext(NULL),
	_currentTime(PHTIMEMIN),
	_useAudio(false),
	_audioStream(NULL),
	_audioFrame(NULL),
	_deinterlace(false),
	_recursive(false)
{
	PHDEBUG << "Using FFMpeg widget for video playback.";
	av_register_all();
	avcodec_register_all();
}

bool PhVideoDecoder::ready()
{
	return (_formatContext && _videoStream && _videoFrame);
}

void PhVideoDecoder::open(QString fileName)
{
	close();
	PHDEBUG << fileName;

	_currentTime = PHTIMEMIN;

	if(avformat_open_input(&_formatContext, fileName.toStdString().c_str(), NULL, NULL) < 0) {
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

	// PhVideoEngine already dumps the stream info, do not do it again here.
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

	emit opened(length(), framePerSecond(), timeIn(), width(), height(), codecName());
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

	// delete all unused buffers
	// Those that are marked as used should not be deleted for now since the engine thread may be
	// operating on them.
	while (_bufferUsageList.contains(false)) {
		int unusedBufferIndex = _bufferUsageList.indexOf(false);
		delete[] _rgbBufferList.takeAt(unusedBufferIndex);
		_bufferSizeList.removeAt(unusedBufferIndex);
		_bufferUsageList.removeAt(unusedBufferIndex);
	}

	_formatContext = NULL;
	_videoStream = NULL;
	_audioStream = NULL;
	PHDEBUG << _fileName << "closed";

	_fileName = "";
}

void PhVideoDecoder::recycleBuffer(uint8_t *rgb)
{
	// mark that this buffer is now available
	int bufferIndex = _rgbBufferList.indexOf(rgb);
	_bufferUsageList.replace(bufferIndex, false);
}

uint8_t *PhVideoDecoder::newRgbBuffer()
{
	// find an available buffer, reuse existing one if possible
	uint8_t * rgb;

	int bufferSize = avpicture_get_size(AV_PIX_FMT_BGRA, width(), height());
	int bufferIndex = _bufferUsageList.indexOf(false);

	if(bufferIndex != -1) {
		// we can reuse an existing available buffer
		rgb = _rgbBufferList.at(bufferIndex);

		if (_bufferSizeList.at(bufferIndex) != bufferSize) {
			// the size has changed, update the buffer
			delete[] rgb;
			rgb = new uint8_t[bufferSize];
			_rgbBufferList.replace(bufferIndex, rgb);
			_bufferSizeList.replace(bufferIndex, bufferSize);
		}
	}
	else {
		// no buffer is currently available, we need a new one
		rgb = new uint8_t[bufferSize];
		_rgbBufferList.append(rgb);
		_bufferUsageList.append(true);
		_bufferSizeList.append(bufferSize);
	}

	return rgb;
}

void PhVideoDecoder::setDeinterlace(bool deinterlace)
{
	PHDEBUG << deinterlace;

	if (deinterlace != _deinterlace) {
		_deinterlace = deinterlace;

		// decode the frame again to apply the new deinterlace setting
		decodeFrame(_currentTime);
	}
}

PhVideoDecoder::~PhVideoDecoder()
{
	close();

	// the engine thread is exiting too, so all the buffers can be cleaned.
	while (!_rgbBufferList.isEmpty())
		delete[] _rgbBufferList.takeFirst();

	_bufferSizeList.clear();
	_bufferUsageList.clear();
}

PhTime PhVideoDecoder::length()
{
	if(_videoStream)
		return AVTimestamp_to_PhTime(_videoStream->duration);
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

void PhVideoDecoder::frameToRgb(uint8_t *rgb)
{
	int frameHeight = _videoFrame->height;
	if(_deinterlace)
		frameHeight = _videoFrame->height / 2;

	// As the following formats are deprecated (see https://libav.org/doxygen/master/pixfmt_8h.html#a9a8e335cf3be472042bc9f0cf80cd4c5)
	// we replace its with the new ones recommended by LibAv
	// in order to get ride of the warnings
	AVPixelFormat pixFormat;
	switch (_videoStream->codec->pix_fmt) {
	case AV_PIX_FMT_YUVJ420P:
		pixFormat = AV_PIX_FMT_YUV420P;
		break;
	case AV_PIX_FMT_YUVJ422P:
		pixFormat = AV_PIX_FMT_YUV422P;
		break;
	case AV_PIX_FMT_YUVJ444P:
		pixFormat = AV_PIX_FMT_YUV444P;
		break;
	case AV_PIX_FMT_YUVJ440P:
		pixFormat = AV_PIX_FMT_YUV440P;
	default:
		pixFormat = _videoStream->codec->pix_fmt;
		break;
	}

	/* Note: we output the frames in AV_PIX_FMT_BGRA rather than AV_PIX_FMT_RGB24,
	 * because this format is native to most video cards and will avoid a conversion
	 * in the video driver */
	/* sws_getCachedContext will check if the context is valid for the given parameters. It the context is not valid,
	 * it will be freed and a new one will be allocated. */
	_swsContext = sws_getCachedContext(_swsContext, _videoFrame->width, _videoStream->codec->height, pixFormat,
	                                   _videoStream->codec->width, frameHeight, AV_PIX_FMT_BGRA,
	                                   SWS_POINT, NULL, NULL, NULL);


	int linesize = _videoFrame->width * 4;
	if (0 <= sws_scale(_swsContext, (const uint8_t * const *) _videoFrame->data,
	                   _videoFrame->linesize, 0, _videoStream->codec->height, &rgb,
	                   &linesize)) {

		PhTime time = AVTimestamp_to_PhTime(av_frame_get_best_effort_timestamp(_videoFrame));

		// tell the video engine that we have finished decoding!
		emit frameAvailable(time, rgb, _videoFrame->width, frameHeight);
	}
}

void PhVideoDecoder::decodeFrame(PhTime time)
{
	if(!ready()) {
		PHDEBUG << "not ready";
		return;
	}

	_requestedTime = time;

	if (_recursive) {
		// if we are called recursively, just let the top caller handle the rest
		return;
	}

	// call processEvents to walk through the queued signals
	// This makes sure we are working on the latest requested time.
	// We use the _recursive flag to indicate that the child slots
	// should not actually decode the frame.
	_recursive = true;
	QCoreApplication::processEvents();
	_recursive = false;

	// at this point _requestedTime is really from the latest requestFrame signal
	time = _requestedTime;

	// find an available buffer
	uint8_t * rgb = newRgbBuffer();

	// clip to stream boundaries
	if(time < 0)
		time = 0;
	if (time >= this->length())
		time = this->length();

	// Stay with the same frame if the time has changed less than the time between two frames
	// Note that av_seek_frame will seek to the _closest_ frame, sometimes a little bit in the "future",
	// so it is necessary to use a little margin for the second comparison, otherwise a seek may
	// be performed on each call to decodeFrame
	if ((time < _currentTime + PhTimeCode::timePerFrame(_tcType))
	    && (time > _currentTime - PhTimeCode::timePerFrame(_tcType)/2)) {
		frameToRgb(rgb);
		return;
	}

	// we need to perform a frame seek if the requested frame is:
	// 1) in the past
	// 2) after the next keyframe
	//      how to know when the next keyframe is ??
	//      -> for now we take a arbitrary threshold of 20 frames
	if((time >= _currentTime + 20*PhTimeCode::timePerFrame(_tcType))
	   || (time < _currentTime)) {
		// seek to the closest keyframe in the past
		int flags = AVSEEK_FLAG_BACKWARD;
		int64_t timestamp = PhTime_to_AVTimestamp(time);
		PHDEBUG << "seek:" << time << " " << _currentTime << " " << time - _currentTime << " " << timestamp << " " << PhTimeCode::timePerFrame(_tcType);
		av_seek_frame(_formatContext, _videoStream->index, timestamp, flags);

		avcodec_flush_buffers(_videoStream->codec);
	}

	AVPacket packet;

	bool lookingForVideoFrame = true;
	while(lookingForVideoFrame) {
		int error = av_read_frame(_formatContext, &packet);
		switch(error) {
		case 0:
			if(packet.stream_index == _videoStream->index) {
				int frameFinished = 0;
				avcodec_decode_video2(_videoStream->codec, _videoFrame, &frameFinished, &packet);
				if(frameFinished) {
					// update the current position of the engine
					// (Note that it is best not to do use '_currentTime = time' here, because the seeking operation may
					// not be 100% accurate: the actual time may be different from the requested time. So a time drift
					// could appear.)
					_currentTime = AVTimestamp_to_PhTime(av_frame_get_best_effort_timestamp(_videoFrame));

					PHDEBUG << time << " " << _currentTime << " " << (time - _currentTime)/PhTimeCode::timePerFrame(_tcType);

					// convert and emit the frame if this is the one that was requested
					if (time < _currentTime + PhTimeCode::timePerFrame(_tcType)) {
						PHDEBUG << "decoded!";
						frameToRgb(rgb);
						lookingForVideoFrame = false;
					}
				} // if frame decode is not finished, let's read another packet.
			}
			else if(_audioStream && (packet.stream_index == _audioStream->index)) {
				int ok = 0;
				avcodec_decode_audio4(_audioStream->codec, _audioFrame, &ok, &packet);
				if(ok) {
					PHDEBUG << "audio:" << _audioFrame->nb_samples;
				}
			}
			break;
		case AVERROR_INVALIDDATA:
		case AVERROR_EOF:
		default:
			{
				char errorStr[256];
				av_strerror(error, errorStr, 256);
				PHDEBUG << time << "error:" << errorStr;
				lookingForVideoFrame = false;
				break;
			}
		}

		//Avoid memory leak
		av_free_packet(&packet);
	}
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

PhTime PhVideoDecoder::timeIn()
{
	PhTime timeIn = 0;

	AVDictionaryEntry *tag = av_dict_get(_formatContext->metadata, "timecode", NULL, AV_DICT_IGNORE_SUFFIX);
	if(tag == NULL)
		tag = av_dict_get(_videoStream->metadata, "timecode", NULL, AV_DICT_IGNORE_SUFFIX);

	if(tag) {
		PHDEBUG << "Found timestamp:" << tag->value;
		timeIn = PhTimeCode::timeFromString(tag->value, _tcType);
	}

	return timeIn;
}

int64_t PhVideoDecoder::PhTime_to_AVTimestamp(PhTime time)
{
	int64_t timestamp = 0;
	if(_videoStream) {
		timestamp = time * _videoStream->time_base.den / _videoStream->time_base.num / PHTIMEBASE;
	}
	return timestamp;
}

PhTime PhVideoDecoder::AVTimestamp_to_PhTime(int64_t timestamp)
{
	PhTime time = 0;
	if(_videoStream)
		time = timestamp * PHTIMEBASE * _videoStream->time_base.num / _videoStream->time_base.den;
	return time;
}
