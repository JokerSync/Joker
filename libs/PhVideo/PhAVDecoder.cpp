/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTime>
#include <qmath.h>

#include "PhAVDecoder.h"

PhAVDecoder::PhAVDecoder(int bufferSize, QObject *parent) :
	QObject(parent),
	_bufferSize(bufferSize),
	_framesFree(bufferSize),
	_firstFrame(0),
	_currentFrame(0),
	_direction(0),
	_pFormatContext(NULL),
	_videoStream(NULL),
	_videoFrame(NULL),
	_pSwsCtx(NULL),
	_videoDeintrelace(false),
	_audioStream(NULL),
	_audioFrame(NULL),
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
	return _firstFrame;
}

void PhAVDecoder::setFirstFrame(PhFrame frame)
{
	_bufferMutex.lock();
	clearBuffer();
	_firstFrame = frame;
	_bufferMutex.unlock();
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
		_bufferMutex.lock();
		clearBuffer();
		_bufferMutex.unlock();
	}
}

int PhAVDecoder::bufferOccupation()
{
	return _bufferSize - _framesFree.available();
}

uint8_t *PhAVDecoder::getBuffer(PhFrame frame)
{
//	PHDEBUG << frame << _framesFree.available();

	uint8_t *buffer = NULL;
	_bufferMutex.lock();
	if(_bufferMap.contains(frame)) {
		buffer = _bufferMap[frame];

		PhFrame minFrame = frame - _bufferSize / 2;
		if(minFrame < _firstFrame)
			minFrame = _firstFrame;
		PhFrame maxFrame = minFrame + _bufferSize;
		foreach (PhFrame key, _bufferMap.keys()) {
			if(key < minFrame) {
				delete _bufferMap[key];
				_bufferMap.remove(key);
				_framesFree.release();
			}
			else if(key > maxFrame) {
				delete _bufferMap[key];
				_bufferMap.remove(key);
				_framesFree.release();
			}
		}
	}

	_bufferMutex.unlock();

	return buffer;
}

void PhAVDecoder::process()
{
	while(!_interupted) {

		if(_direction == -1) {
			_bufferMutex.lock();
			// If the buffer contains the current frame, seek to the last key frame
			if(_bufferMap.contains(_currentFrame)) {
				// Look for the minimun frame
				foreach(PhFrame key, _bufferMap.keys()) {
					if(key < _currentFrame)
						_currentFrame = key;
				}
				int flags = AVSEEK_FLAG_BACKWARD;
				int64_t timestamp = frame2time(_currentFrame - _firstFrame);
				PHDEBUG << "seek:" << _direction << _currentFrame;
				av_seek_frame(_pFormatContext, _videoStream->index, timestamp, flags);
			}
			_bufferMutex.unlock();
		}

		AVPacket packet;

		bool lookingForVideoFrame = _currentFrame < _firstFrame + _videoStream->duration;
		while(lookingForVideoFrame) {
			_bufferMutex.lock();
			int error = av_read_frame(_pFormatContext, &packet);
			_bufferMutex.unlock();
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
							_bufferMutex.lock();
							_bufferMap[_currentFrame] = rgb;
							//							PHDEBUG << _currentFrame << rgb << packet.dts << _framesFree.available() << _framesProcessed.available();
							_bufferMutex.unlock();
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

		_currentFrame ++;
	}

	PHDEBUG << "Bye bye";
	emit finished();
}

void PhAVDecoder::onFrameChanged(PhFrame frame, PhTimeCodeType tcType)
{
	_bufferMutex.lock();
	if(!_bufferMap.contains(frame)) {
		clearBuffer();

	}
	_bufferMutex.unlock();
}

void PhAVDecoder::onRateChanged(PhRate rate)
{
	if(rate < 0)
		_direction = -1;
	else if (rate>0)
		_direction = 1;
	else
		_direction = 0;
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
	PHDEBUG;
	qDeleteAll(_bufferMap);
	_bufferMap.clear();
	_framesFree.release(_bufferSize - _framesFree.available());
}

