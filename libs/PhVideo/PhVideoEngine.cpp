/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhTools/PhGeneric.h"
#include "PhTools/PhDebug.h"

#include "PhVideoEngine.h"

PhVideoEngine::PhVideoEngine(PhVideoSettings *settings) :
	_settings(settings),
	_fileName(""),
	_tcType(PhTimeCodeType25),
	_timeIn(0),
	_formatContext(NULL),
	_videoStream(NULL),
	_videoFrame(NULL),
	_swsContext(NULL),
	_currentTime(PHTIMEMIN),
	_useAudio(false),
	_audioStream(NULL),
	_audioFrame(NULL),
	_deinterlace(false),
	_rgb(NULL)
{
	PHDEBUG << "Using FFMpeg widget for video playback.";
	av_register_all();
	avcodec_register_all();
}

bool PhVideoEngine::ready()
{
	return (_formatContext && _videoStream && _videoFrame);
}

void PhVideoEngine::setDeinterlace(bool deinterlace)
{
	PHDEBUG << deinterlace;
	_deinterlace = deinterlace;
	if(_rgb) {
		delete[] _rgb;
		_rgb = NULL;
	}
	_currentTime = PHTIMEMIN;
}

bool PhVideoEngine::bilinearFiltering()
{
	return _videoRect.bilinearFiltering();
}

void PhVideoEngine::setBilinearFiltering(bool bilinear)
{
	_videoRect.setBilinearFiltering(bilinear);
}

bool PhVideoEngine::open(QString fileName)
{
	close();
	PHDEBUG << fileName;

	_clock.setTime(0);
	_clock.setRate(0);
	_currentTime = PHTIMEMIN;

	if(avformat_open_input(&_formatContext, fileName.toStdString().c_str(), NULL, NULL) < 0)
		return false;

	PHDEBUG << "Retrieve stream information";
	if (avformat_find_stream_info(_formatContext, NULL) < 0)
		return false; // Couldn't find stream information

	av_dump_format(_formatContext, 0, fileName.toStdString().c_str(), 0);

	// Find video stream :
	for(int i = 0; i < (int)_formatContext->nb_streams; i++) {
		AVMediaType streamType = _formatContext->streams[i]->codec->codec_type;
		PHDEBUG << i << ":" << streamType;
		switch(streamType) {
		case AVMEDIA_TYPE_VIDEO:
			_videoStream = _formatContext->streams[i];
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

	if(_videoStream == NULL)
		return false;

	// Looking for timecode type
	_tcType = PhTimeCode::computeTimeCodeType(this->framePerSecond());
	emit timeCodeTypeChanged(_tcType);

	// Reading timestamp :
	AVDictionaryEntry *tag = av_dict_get(_formatContext->metadata, "timecode", NULL, AV_DICT_IGNORE_SUFFIX);
	if(tag == NULL)
		tag = av_dict_get(_videoStream->metadata, "timecode", NULL, AV_DICT_IGNORE_SUFFIX);

	if(tag) {
		PHDEBUG << "Found timestamp:" << tag->value;
		_timeIn = PhTimeCode::timeFromString(tag->value, _tcType);
	}


	PHDEBUG << "size : " << _videoStream->codec->width << "x" << _videoStream->codec->height;
	AVCodec * videoCodec = avcodec_find_decoder(_videoStream->codec->codec_id);
	if(videoCodec == NULL) {
		PHDEBUG << "Unable to find the codec:" << _videoStream->codec->codec_id;
		return false;
	}


	if (avcodec_open2(_videoStream->codec, videoCodec, NULL) < 0) {
		PHDEBUG << "Unable to open the codec:" << _videoStream->codec;
		return false;
	}

	_videoFrame = av_frame_alloc();

	PHDEBUG << "length:" << this->length();
	PHDEBUG << "fps:" << this->framePerSecond();

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

	decodeFrame(0);
	_fileName = fileName;

	return true;
}

void PhVideoEngine::close()
{
	PHDEBUG << _fileName;
	if(_rgb) {
		delete[] _rgb;
		_rgb = NULL;
	}

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

	_timeIn = 0;
	_formatContext = NULL;
	_videoStream = NULL;
	_audioStream = NULL;
	PHDEBUG << _fileName << "closed";

	_fileName = "";
}

void PhVideoEngine::drawVideo(int x, int y, int w, int h)
{
	if(_videoStream) {
		PhTime delay = static_cast<PhTime>(_settings->screenDelay() * _clock.rate() * 24);
		decodeFrame(_clock.time() + delay);
	}

	if(_settings->useNativeVideoSize())
		_videoRect.setRect(x, y, this->width(), this->height());
	else
		_videoRect.setRect(x, y, w, h);
	_videoRect.setZ(-10);
	_videoRect.draw();
}

void PhVideoEngine::setTimeIn(PhTime timeIn)
{
	PHDEBUG << timeIn;
	_timeIn = timeIn;
}

PhTime PhVideoEngine::length()
{
	if(_videoStream)
		return AVTimestamp_to_PhTime(_videoStream->duration);
	return 0;
}

PhVideoEngine::~PhVideoEngine()
{
	close();
}

int PhVideoEngine::width()
{
	if(_videoStream)
		return _videoStream->codec->width;
	return 0;
}

int PhVideoEngine::height()
{
	if(_videoStream)
		return _videoStream->codec->height;
	return 0;
}

double PhVideoEngine::framePerSecond()
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

QString PhVideoEngine::codecName()
{
	if(_videoStream)
		return _videoStream->codec->codec_descriptor->long_name;

	return "";
}

bool PhVideoEngine::decodeFrame(PhTime time)
{
	if(!ready()) {
		PHDEBUG << "not ready";
		return false;
	}

	if(time < _timeIn)
		time = _timeIn;
	if (time >= this->timeOut())
		time = this->timeOut();

	bool result = false;

	// Stay with the same frame if the time has changed less than the time between two frames
	// Note that av_seek_frame will seek to the _closest_ frame, sometimes a little bit in the "future",
	// so it is necessary to use a little margin for the second comparison, otherwise a seek may
	// be performed on each call to decodeFrame
	if ((time < _currentTime + PhTimeCode::timePerFrame(_tcType))
	    && (time > _currentTime - PhTimeCode::timePerFrame(_tcType)/2))
		result = true;
	else {
		// we need to perform a frame seek if the requested frame is not the next frame in the stream
		if((time >= _currentTime + 2*PhTimeCode::timePerFrame(_tcType))
		   || (time < _currentTime)) {
			int flags = AVSEEK_FLAG_ANY;
			int64_t timestamp = PhTime_to_AVTimestamp(time - _timeIn);
			PHDEBUG << "seek:" << time << " " << _currentTime << " " << _timeIn << " " << timestamp;
			av_seek_frame(_formatContext, _videoStream->index, timestamp, flags);
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

						if(_rgb == NULL)
							_rgb = new uint8_t[avpicture_get_size(AV_PIX_FMT_BGRA, _videoFrame->width, frameHeight)];
						int linesize = _videoFrame->width * 4;
						if (0 <= sws_scale(_swsContext, (const uint8_t * const *) _videoFrame->data,
						                   _videoFrame->linesize, 0, _videoStream->codec->height, &_rgb,
						                   &linesize)) {

							_videoRect.createTextureFromBGRABuffer(_rgb, _videoFrame->width, frameHeight);


							_videoFrameTickCounter.tick();
							result = true;
						}
						lookingForVideoFrame = false;
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

			// update the current position of the engine
			// (Note that it is best not to do use '_currentTime = time' here, because the seeking operation may
			// not be 100% accurate: the actual time may be different from the requested time. So a time drift
			// could appear.)
			_currentTime = _timeIn + AVTimestamp_to_PhTime(av_frame_get_best_effort_timestamp(_videoFrame));

			//Avoid memory leak
			av_free_packet(&packet);
		}
	}

	return result;
}

int64_t PhVideoEngine::PhTime_to_AVTimestamp(PhTime time)
{
	int64_t timestamp = 0;
	if(_videoStream) {
		timestamp = static_cast<int64_t>(std::round(static_cast<double>(time) / 24000. / av_q2d(_videoStream->time_base)));
	}
	return timestamp;
}

PhTime PhVideoEngine::AVTimestamp_to_PhTime(int64_t timestamp)
{
	PhTime time = 0;
	if(_videoStream) {
		time = static_cast<PhTime>(std::round(static_cast<double>(timestamp) * av_q2d(_videoStream->time_base) * 24000.));
	}
	return time;
}
