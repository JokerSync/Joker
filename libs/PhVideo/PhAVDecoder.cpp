/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTime>

#include "PhAVDecoder.h"

PhAVDecoder::PhAVDecoder(QObject *parent) :
	QObject(parent),
	_framesProcessed(0),
	_framesFree(100),
	_pFormatContext(NULL),
	_videoStream(NULL),
	_audioStream(NULL),
	_pSwsCtx(NULL),
	_rate(1),
	_videoDeintrelace(false),
	_interupted(false)
{
}

PhAVDecoder::~PhAVDecoder()
{
	clearBuffer();

	avformat_close_input(&_pFormatContext);
}

bool PhAVDecoder::open(QString fileName)
{
	PHDEBUG << fileName;
	if(avformat_open_input(&_pFormatContext, fileName.toStdString().c_str(), NULL, NULL) < 0)
		return false;

	// Retrieve stream information
	if (avformat_find_stream_info(_pFormatContext, NULL) < 0)
		return false; // Couldn't find stream information

	av_dump_format(_pFormatContext, 0, fileName.toStdString().c_str(), 0);

	_firstFrame = 0;
	_videoStream = NULL;
	_audioStream = NULL;

	// Find video stream
	for(int i = 0; i < (int)_pFormatContext->nb_streams; i++) {
		AVMediaType streamType = _pFormatContext->streams[i]->codec->codec_type;
		PHDEBUG << i << ":" << streamType;
		switch(streamType) {
		case AVMEDIA_TYPE_VIDEO:
			_videoStream = _pFormatContext->streams[i];
			PHDEBUG << "\t=> video";
			break;
		case AVMEDIA_TYPE_AUDIO:
			if(_audioStream == NULL)
				_audioStream = _pFormatContext->streams[i];
			PHDEBUG << "\t=> audio";
			break;
		default:
			PHDEBUG << "\t=> unknown";
			break;
		}
	}

	if(_videoStream == NULL)
		return false;

	// Reading timestamp :
	AVDictionaryEntry *tag = av_dict_get(_pFormatContext->metadata, "timecode", NULL, AV_DICT_IGNORE_SUFFIX);
	if(tag == NULL)
		tag = av_dict_get(_videoStream->metadata, "timecode", NULL, AV_DICT_IGNORE_SUFFIX);

	if(tag) {
		PHDEBUG << "Found timestamp:" << tag->value;
		_firstFrame = PhTimeCode::frameFromString(tag->value, PhTimeCodeType25);
	}

	PHDEBUG << "length:" << this->length();
	PHDEBUG << "fps:" << this->framePerSecond();

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

	_videoFrame = avcodec_alloc_frame();

	_currentFrame = _firstFrame;

	if(_audioStream) {
		AVCodec* audioCodec = avcodec_find_decoder(_audioStream->codec->codec_id);
		if(audioCodec) {
			if(avcodec_open2(_audioStream->codec, audioCodec, NULL) < 0) {
				PHDEBUG << "Unable to open audio codec.";
				_audioStream = NULL;
			}
			else {
				_audioFrame = avcodec_alloc_frame();
				PHDEBUG << "Audio OK.";
			}
		}
		else {
			PHDEBUG << "Unable to find codec for audio.";
			_audioStream = NULL;
		}
	}
	return true;
}

void PhAVDecoder::close()
{
	PHDEBUG;

	_interupted = true;
	_framesFree.release();
}

uint8_t *PhAVDecoder::getBuffer(PhFrame frame)
{
	if(frame > _lastAskedFrame)
		_rate = 1;
	else
		_rate = -1;

	uint8_t *buffer = NULL;
	if(frame < _firstFrame + _videoStream->duration && _framesProcessed.tryAcquire()) {
		_nextImagesMutex.lock();
		if(!_nextImages.contains(frame)) {
			_currentFrame = frame;
			clearBuffer();
		}
		else {
			buffer = _nextImages[frame];
			_nextImages.remove(frame);
			_framesFree.release();
		}

		_nextImagesMutex.unlock();
		_lastAskedFrame = frame;
	}
	return buffer;
}

PhTimeCodeType PhAVDecoder::timeCodeType()
{
	// Looking for timecode type
	float fps = framePerSecond();

	if(fps == 0) {
		PHDEBUG << "Bad fps detect => assuming 25";
		return PhTimeCodeType25;
	}
	else if(fps < 24)
		return PhTimeCodeType2398;
	else if (fps < 24.5f)
		return PhTimeCodeType24;
	else if (fps < 26)
		return PhTimeCodeType25;
	else if (fps < 30)
		return PhTimeCodeType2997;
	else {
#warning /// @todo patch for #107 => find better fps decoding
		PHDEBUG << "Bad fps detect => assuming 25";
		return PhTimeCodeType25;
	}
}

PhFrame PhAVDecoder::firstFrame()
{
	// Reading timestamp :
	AVDictionaryEntry *tag = av_dict_get(_pFormatContext->metadata, "timecode", NULL, AV_DICT_IGNORE_SUFFIX);
	if(tag == NULL)
		tag = av_dict_get(_videoStream->metadata, "timecode", NULL, AV_DICT_IGNORE_SUFFIX);

	if(tag) {
		PHDEBUG << "Found timestamp:" << tag->value;
		_firstFrame = PhTimeCode::frameFromString(tag->value, timeCodeType());
	}
	return _firstFrame;
}

int PhAVDecoder::width()
{
	if(_videoStream)
		return _videoStream->codec->width;
	else
		return 0;
}

int PhAVDecoder::height()
{
	if(_videoStream)
		return _videoStream->codec->height;
	else
		return 0;
}

PhFrame PhAVDecoder::length()
{
	if(_videoStream)
		return time2frame(_videoStream->duration);
	return 0;
}

float PhAVDecoder::framePerSecond()
{
	float fps = 0;
	if(_videoStream) {
		fps = _videoStream->avg_frame_rate.num;
		fps /= _videoStream->avg_frame_rate.den;
	}
	return fps;
}

QString PhAVDecoder::codecName()
{
	if(_videoStream)
		return _videoStream->codec->codec_name;
	return "";
}

void PhAVDecoder::setDeintrelace(bool deintrelace)
{
	if(deintrelace != _videoDeintrelace) {
		_videoDeintrelace = deintrelace;
		_nextImagesMutex.lock();
		clearBuffer();
		_nextImagesMutex.unlock();
	}
}

void PhAVDecoder::process()
{
	while(!_interupted) {

		if(_rate == -1 || (abs(_currentFrame - _lastAskedFrame) > 1)) {
			int flags = AVSEEK_FLAG_ANY;
			int64_t timestamp = frame2time(_currentFrame - _firstFrame);
			//PHDEBUG << "seek:" << _currentFrame;
			av_seek_frame(_pFormatContext, _videoStream->index, timestamp, flags);
		}


		AVPacket packet;

		bool lookingForVideoFrame = _currentFrame < _firstFrame + _videoStream->duration;
		while(lookingForVideoFrame) {
			int error = av_read_frame(_pFormatContext, &packet);
			switch(error) {
			case 0:
				if(packet.stream_index == _videoStream->index) {

					int frameFinished = 0;
					avcodec_decode_video2(_videoStream->codec, _videoFrame, &frameFinished, &packet);
					if(frameFinished) {

						int frameHeight = _videoFrame->height;
						if(_videoDeintrelace)
							frameHeight /= 2;

#warning /// @todo Use RGB pixel format
						_pSwsCtx = sws_getCachedContext(_pSwsCtx, _videoFrame->width, _videoStream->codec->height,
						                                _videoStream->codec->pix_fmt, _videoStream->codec->width, frameHeight,
						                                AV_PIX_FMT_RGBA, SWS_POINT, NULL, NULL, NULL);

						uint8_t * rgb = new uint8_t[_videoFrame->width * frameHeight * 4];
						int linesize = _videoFrame->width * 4;
						if (0 <= sws_scale(_pSwsCtx, (const uint8_t * const *) _videoFrame->data,
						                   _videoFrame->linesize, 0, _videoStream->codec->height, &rgb,
						                   &linesize)) {
							_framesFree.acquire();
							_nextImagesMutex.lock();
							_nextImages[_currentFrame] = rgb;
							_nextImagesMutex.unlock();
						}
						lookingForVideoFrame = false;
					} // if frame decode is not finished, let's read another packet.
				}
				else if(_audioStream && (packet.stream_index == _audioStream->index)) {
					int ok = 0;
					avcodec_decode_audio4(_audioStream->codec, _audioFrame, &ok, &packet);
					if(ok) {
						//PHDEBUG << "audio:" << _audioFrame->nb_samples;
					}
				}
				break;
			case AVERROR_INVALIDDATA:
			case AVERROR_EOF:
			default:
				{
					char errorStr[256];
					av_strerror(error, errorStr, 256);
					PHDEBUG << _currentFrame << "error:" << errorStr;
					lookingForVideoFrame = false;
					break;
				}
			}
			//Avoid memory leak
			av_free_packet(&packet);
		}

//		As rate == -1 or 1
//		if(_rate == 1) {
//			_currentFrame++;
//		}
//		else {
//			_currentFrame--;
//		}
		_currentFrame += _rate;
		_framesProcessed.release();
	}

	PHDEBUG << "Bye bye";
	emit finished();
}

int64_t PhAVDecoder::frame2time(PhFrame f)
{
	int64_t t = 0;
	if(_videoStream) {
		PhFrame fps = PhTimeCode::getFps(PhTimeCodeType25);
		t = f * _videoStream->time_base.den / _videoStream->time_base.num / fps;
	}
	return t;
}

PhFrame PhAVDecoder::time2frame(int64_t t)
{
	PhFrame f = 0;
	if(_videoStream) {
		PhFrame fps = PhTimeCode::getFps(PhTimeCodeType25);
		f = t * _videoStream->time_base.num * fps / _videoStream->time_base.den;
	}
	return f;
}

void PhAVDecoder::clearBuffer()
{
	qDeleteAll(_nextImages);
	_nextImages.clear();
	_framesFree.release(100 - _framesFree.available());
	_framesProcessed.acquire(_framesProcessed.available());
}

