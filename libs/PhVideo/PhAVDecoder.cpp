/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <QTime>
#include <qmath.h>
#include <QCoreApplication>
#include <QThread>


#include "PhAVDecoder.h"

PhAVDecoder::PhAVDecoder(int bufferSize, QObject *parent) :
	QObject(parent),
	_bufferSize(bufferSize),
	_bufferFreeSpace(bufferSize),
	_firstFrame(0),
	_nextDecodingFrame(0),
	_direction(0),
	_pFormatContext(NULL),
	_videoStream(NULL),
	_videoFrame(NULL),
	_deinterlace(false),
	_audioStream(NULL),
	_audioFrame(NULL),
	_interupted(false),
	_lastDecodedFrame(0),
	_oldFrame(0)
{
	PHDEBUG << "Setting the decoder with a buffer of" << bufferSize << "frames";
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

	_nextDecodingFrame = _firstFrame;

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
	PHDEBUG << frame;
	_bufferMutex.lock();
	clearBuffer();
	_nextDecodingFrame = _firstFrame = frame;
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
		// See http://stackoverflow.com/a/570694/2307070
		// for NaN handling
		if(fps != fps) {
			fps = _videoStream->time_base.den;
			fps /= _videoStream->time_base.num;
		}
	}

	return fps;
}

QString PhAVDecoder::codecName()
{
	if(_videoStream)
		return _videoStream->codec->codec_name;
	return "";
}

void PhAVDecoder::setDeinterlace(bool deintrelace)
{
	if(deintrelace != _deinterlace) {
		_deinterlace = deintrelace;
		_bufferMutex.lock();
		clearBuffer();
		_nextDecodingFrame = _oldFrame;
		_bufferMutex.unlock();
	}
}

int PhAVDecoder::bufferOccupation()
{
	return _bufferSize - _bufferFreeSpace.available();
}

uint8_t *PhAVDecoder::getBuffer(PhFrame frame)
{
	//PHDBG(24) << frame << _framesFree.available();
	uint8_t *buffer = NULL;
	_bufferMutex.lock();
	if(_bufferMap.contains(frame)) {
		buffer = _bufferMap[frame];
	}
	_bufferMutex.unlock();

	return buffer;
}

void PhAVDecoder::process()
{
	while(!_interupted) {
		QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
		// If the decoder is blocked
		while (!_bufferFreeSpace.tryAcquire()) {
			// Process the events (max time 5ms)
			QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
			// Decrease CPU load if the player is in pause
			if(_direction == 0)
				QThread::msleep(5);
		}
		//QTime t;
		//t.start();
		decodeFrame(_nextDecodingFrame);
		//PHDEBUG << t.elapsed() << "ms to decode the frame" << _nextDecodingFrame;
		switch (_direction) {
		case 1:
		case 0:
			_nextDecodingFrame++;
			break;
		case -1:
			_nextDecodingFrame--;
		default:
			break;
		}
	}
	PHDEBUG << "Bye bye";
	emit finished();
}

void PhAVDecoder::decodeFrame(PhFrame frame)
{
	// Exit if the frame is already in the buffer
	_bufferMutex.lock();
	if(_bufferMap.contains(frame)) {
		_bufferMutex.unlock();
		//Release the unused ressource
		_bufferFreeSpace.release();
		return;
	}
	_bufferMutex.unlock();

	if(frame - _lastDecodedFrame != 1) {
		int flags = AVSEEK_FLAG_ANY;
		int64_t timestamp = frame2time(frame - _firstFrame);
		PHDEBUG << "seek:" << _direction << frame;
		av_seek_frame(_pFormatContext, _videoStream->index, timestamp, flags);
	}

	int frameFinished = 0;
	while(frameFinished == 0) {
		AVPacket packet;
		int error = av_read_frame(_pFormatContext, &packet);
		switch(error) {
		case 0:
			if(packet.stream_index == _videoStream->index) {
				avcodec_decode_video2(_videoStream->codec, _videoFrame, &frameFinished, &packet);
				if(frameFinished) {

					int frameHeight = _videoFrame->height;
					if(_deinterlace)
						frameHeight /= 2;
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
					SwsContext * swsContext = sws_getContext(_videoFrame->width, _videoFrame->height, pixFormat,
					                                         _videoFrame->width, frameHeight, AV_PIX_FMT_RGB24,
					                                         SWS_POINT, NULL, NULL, NULL);

					uint8_t * rgb = new uint8_t[_videoFrame->width * frameHeight * 3];
					int linesize = _videoFrame->width * 3;
					if (0 <= sws_scale(swsContext, (const uint8_t * const *) _videoFrame->data,
					                   _videoFrame->linesize, 0, _videoFrame->height, &rgb,
					                   &linesize)) {
						_bufferMutex.lock();
						_bufferMap[frame] = rgb;
						//PHDBG(25) << "Decoding" << PhTimeCode::stringFromFrame(frame, PhTimeCodeType25) << packet.dts << _bufferFreeSpace.available();
						_bufferMutex.unlock();
						_lastDecodedFrame = frame;
						//PHDEBUG << "Add" << frame;
					}
				}     // if frame decode is not finished, let's read another packet.
			}
			else if(_audioStream && (packet.stream_index == _audioStream->index)) {
				int ok = 0;
				avcodec_decode_audio4(_audioStream->codec, _audioFrame, &ok, &packet);
				//				if(ok) {
				//					PHDEBUG << "audio:" << _audioFrame->nb_samples;
				//				}
			}
			break;
		case AVERROR_EOF:
			// As we reach the end of the file, it's useless to go full speed
			QThread::msleep(10);
		case AVERROR_INVALIDDATA:
		default:
			{
				char errorStr[256];
				av_strerror(error, errorStr, 256);
				PHDEBUG << "error on frame" << frame << ":" << errorStr;
				// In order to get out of the while in case of error
				frameFinished = -1;
				break;
			}
		}
		//Avoid memory leak
		av_free_packet(&packet);
	}
}

void PhAVDecoder::onRateChanged(PhRate rate)
{
	if(rate < 0)
		_direction = -1;
	else if (rate > 0)
		_direction = 1;
	else
		_direction = 0;
	PHDEBUG << "Direction is now" <<_direction;
}

void PhAVDecoder::onFrameChanged(PhFrame frame, PhTimeCodeType)
{
	_bufferMutex.lock();
//	PHDBG(25) << "Want" << frame << _direction << "Have ("<<  _bufferMap.count() <<":" << _bufferFreeSpace.available() << "):" << _bufferMap.keys();
	if(!_bufferMap.contains(frame))
		_nextDecodingFrame = frame;
	if(_direction >= 0) {
		// Remove old frames
		PhFrame min = PHFRAMEMAX;
		foreach(PhFrame key, _bufferMap.keys()) {
			if(key < frame - _bufferSize / 2) {
				delete _bufferMap[key];
				_bufferMap.remove(key);
				_bufferFreeSpace.release();
				//PHDEBUG << "Remove" << key;
			}
			if(key < min)
				min = key;
		}
		if(min > frame) {
			clearBuffer();
			_nextDecodingFrame = frame;
		}
	}
	else {
		// Remove future frames
		PhFrame max = PHFRAMEMIN;
		foreach(PhFrame key, _bufferMap.keys()) {
			if(key > frame + _bufferSize / 2) {
				delete _bufferMap[key];
				_bufferMap.remove(key);
				_bufferFreeSpace.release();
			}
			if(key > max)
				max = key;
		}
		if(max < frame) {
			clearBuffer();
			_nextDecodingFrame = frame;
		}
	}
	_oldFrame = frame;
//	PHDEBUG << "Should decode:" << _nextDecodingFrame << "Have ("<<  _bufferMap.count() <<":" << _bufferFreeSpace.available() << "):" << _bufferMap.keys();
	_bufferMutex.unlock();
}

int64_t PhAVDecoder::frame2time(PhFrame f)
{
	int64_t t = 0;
	if(_videoStream) {
		PhFrame fps = PhTimeCode::getFps(timeCodeType());
		t = f * _videoStream->time_base.den / _videoStream->time_base.num / fps;
	}
	return t;
}

PhFrame PhAVDecoder::time2frame(int64_t t)
{
	PhFrame f = 0;
	if(_videoStream) {
		PhFrame fps = PhTimeCode::getFps(timeCodeType());
		f = t * _videoStream->time_base.num * fps / _videoStream->time_base.den;
	}
	return f;
}

void PhAVDecoder::clearBuffer()
{
	PHDEBUG << "Clearing the buffer";
	qDeleteAll(_bufferMap);
	_bufferFreeSpace.release(_bufferMap.count());
	_bufferMap.clear();

}


