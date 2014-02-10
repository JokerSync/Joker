#include "PhVideoEngine.h"

PhVideoEngine::PhVideoEngine(bool useAudio, QObject *parent) :	QObject(parent),
	_settings(NULL),
	_fileName(""),
	_clock(PhTimeCodeType25),
	_firstFrame(0),
	_pFormatContext(NULL),
	_videoStream(NULL),
	_videoFrame(NULL),
	_pSwsCtx(NULL),
	_rgb(NULL),
	_currentFrame(-1),
	_useAudio(true),
	_audioStream(NULL),
	_audioFrame(NULL)
{
	PHDEBUG << "Using FFMpeg widget for video playback.";
	av_register_all();
	_testTimer.start();
}

bool PhVideoEngine::init(int nbChannels, AVSampleFormat sampleFormat, int sampleRate, int framePerBuffer, QString deviceName)
{
	PHDBG(0) << nbChannels << sampleFormat << sampleRate << framePerBuffer << deviceName;

	PaError err = Pa_Initialize();
	if( err != paNoError )
		return false;

	int deviceCount = Pa_GetDeviceCount();
	if( deviceCount <= 0 )
	{
		PHDBG(0) << "ERROR: Pa_CountDevices returned " << deviceCount;
		return false;
	}


	PaStreamParameters outputDeviceInfo;
	outputDeviceInfo.device = Pa_GetDefaultOutputDevice();
	outputDeviceInfo.channelCount = nbChannels;
	outputDeviceInfo.suggestedLatency = 0;
	outputDeviceInfo.hostApiSpecificStreamInfo = NULL;

	switch (sampleFormat) {
	case AV_SAMPLE_FMT_S16:
		outputDeviceInfo.sampleFormat = paInt16;
		break;
	case AV_SAMPLE_FMT_S32:
		outputDeviceInfo.sampleFormat = paInt32;
		break;
#warning TODO Handle more AVSampleFormat
	case AV_SAMPLE_FMT_FLT:
	case AV_SAMPLE_FMT_DBL:
	case AV_SAMPLE_FMT_U8:
	default:
		PHDBG(0) << "Unsupported AVSampleFormat value : " << sampleFormat;
		return false;
	}

	bool isThereOutput = false;
	bool deviceFound = false;

	for(int i = 0; i < deviceCount; i++ )
	{
		const PaDeviceInfo *deviceInfo;
		deviceInfo = Pa_GetDeviceInfo( i );
		if(deviceInfo->maxOutputChannels >= nbChannels )
		{
			isThereOutput = true;
			if(deviceName == deviceInfo->name)
			{
				deviceFound = true;
				outputDeviceInfo.device = i;
				break;
			}
		}
	}
	if(!isThereOutput)
	{
		PHDBG(0) << "No output device";
		return false;
	}
	if(deviceName.length() and !deviceFound)
	{
		PHDBG(0) << "Desired output not found :" << deviceName;
		return false;
	}


	err = Pa_OpenStream(&stream, NULL, &outputDeviceInfo, sampleRate, framePerBuffer, paNoFlag, audioCallback, this);

	if(err != paNoError)
	{
		PHDBG(0) << "Error while opening the stream : " << Pa_GetErrorText(err);
		return false;
	}

	err = Pa_StartStream( stream );
	if(err != paNoError)
	{
		PHDBG(0) << "Error while opening the stream : " << Pa_GetErrorText(err);
		return false;
	}

	PHDBG(0) <<"Port audio succeed initialization !";
	return true;
}

bool PhVideoEngine::ready()
{
	return (_pFormatContext && _videoStream && _videoFrame);
}

bool PhVideoEngine::open(QString fileName)
{
	close();
	PHDEBUG << fileName;
	if(avformat_open_input(&_pFormatContext, fileName.toStdString().c_str(), NULL, NULL) < 0)
		return false;

	// Retrieve stream information
	if (avformat_find_stream_info(_pFormatContext, NULL) < 0)
		return -1; // Couldn't find stream information

	av_dump_format(_pFormatContext, 0, fileName.toStdString().c_str(), 0);

	_firstFrame = 0;
	_videoStream = NULL;
	_audioStream = NULL;


	// Find video stream :
	for(int i = 0; i < (int)_pFormatContext->nb_streams; i++)
	{
		AVMediaType streamType = _pFormatContext->streams[i]->codec->codec_type;
		PHDEBUG << i << ":" << streamType;
		switch(streamType)
		{
		case AVMEDIA_TYPE_VIDEO:
			_videoStream = _pFormatContext->streams[i];
			PHDEBUG << "\t=> video";
			break;
		case AVMEDIA_TYPE_AUDIO:
			if(_useAudio && (_audioStream == NULL))
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

	if(tag)
	{
		PHDEBUG << "Found timestamp:" << tag->value;
		_firstFrame = PhTimeCode::frameFromString(tag->value, _clock.timeCodeType());
	}

	// Looking for timecode type
	float fps = this->framePerSecond();
	if(fps < 24)
		_clock.setTimeCodeType(PhTimeCodeType2398);
	else if (fps < 24.5f)
		_clock.setTimeCodeType(PhTimeCodeType24);
	else if (fps < 26)
		_clock.setTimeCodeType(PhTimeCodeType25);
	else
		_clock.setTimeCodeType(PhTimeCodeType2997);

	PHDEBUG << "size : " << _videoStream->codec->width << "x" << _videoStream->codec->height;
	AVCodec * videoCodec = avcodec_find_decoder(_videoStream->codec->codec_id);
	if(videoCodec == NULL)
		return false;

	if(avcodec_open2(_videoStream->codec, videoCodec, NULL) < 0)
		return false;

	_videoFrame = avcodec_alloc_frame();

	PHDEBUG << "length:" << this->length();
	_currentFrame = -1;
	_clock.setFrame(0);

	if(_audioStream)
	{
		AVCodec* audioCodec = avcodec_find_decoder(_audioStream->codec->codec_id);
		if(audioCodec)
		{
			if(avcodec_open2(_audioStream->codec, audioCodec, NULL) < 0)
			{
				PHDEBUG << "Unable to open audio codec.";
				_audioStream = NULL;
			}
			else
			{
				_audioFrame = avcodec_alloc_frame();
				init(_audioStream->codec->channels, _audioStream->codec->sample_fmt, _audioStream->codec->sample_rate, 1024);
			}
		}
		else
		{
			PHDEBUG << "Unable to find codec for audio.";
			_audioStream = NULL;
		}
	}

	//goToFrame(0);
	_fileName = fileName;
	return true;
}

void PhVideoEngine::close()
{
	PHDEBUG;
	if(_rgb)
	{
		delete _rgb;
		_rgb = NULL;
	}

	if(_pFormatContext)
	{
		avformat_close_input(&_pFormatContext);
		_pFormatContext = NULL;
		_videoStream = NULL;
	}

	_fileName = "";

	Pa_CloseStream(stream);
}

void PhVideoEngine::setSettings(QSettings *settings)
{
	_settings = settings;
}

void PhVideoEngine::drawVideo(int x, int y, int w, int h)
{
//	_clock.tick(60);
	PhFrame delay = 0;
	if(_settings)
		delay = _settings->value("delay", 0).toInt() * PhTimeCode::getFps(_clock.timeCodeType()) * _clock.rate() / 1000;
	//goToFrame(_clock.frame() + delay);
	videoRect.setRect(x, y, w, h);
	videoRect.setZ(-10);
	videoRect.draw();
}

PhFrame PhVideoEngine::length()
{
	if(_videoStream)
		return time2frame(_videoStream->duration);
	return 0;
}

void PhVideoEngine::setFirstFrame(PhFrame frame)
{
	_firstFrame = frame;
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

float PhVideoEngine::framePerSecond()
{
	float result = 0;
	if(_videoStream)
	{
		result = _videoStream->avg_frame_rate.num;
		result /= _videoStream->avg_frame_rate.den;
	}

	return result;
}

QString PhVideoEngine::codecName()
{
	if(_videoStream)
		return _videoStream->codec->codec_name;
	return "";
}

bool PhVideoEngine::goToFrame(PhFrame frame)
{
//	int lastGotoElapsed = _testTimer.elapsed();
	int seekElapsed = -1;
	int readElapsed = -1;
	int decodeElapsed = -1;
	int scaleElapsed = -1;
	int textureElapsed = -1;

	if(!ready())
	{
		PHDEBUG << "not ready";
		return false;
	}

	if(frame < this->firstFrame())
		frame = this->firstFrame();
	if (frame >= this->lastFrame())
		frame = this->lastFrame();

	bool result = false;
	// Do not perform frame seek if the rate is 0 and the last frame is the same frame
	if (frame == _currentFrame)
		result = true;
	else
	{
		// Do not perform frame seek if the rate is 1 and the last frame is the previous frame
		if(frame - _currentFrame != 1)
		{
			int flags = AVSEEK_FLAG_ANY;
			int64_t timestamp = frame2time(frame - _firstFrame);
			PHDEBUG << "seek:" << timestamp << _videoStream->time_base.num << _videoStream->time_base.den;
			av_seek_frame(_pFormatContext, _videoStream->index, timestamp, flags);
		}

		seekElapsed = _testTimer.elapsed();

		AVPacket packet;

		bool lookingForVideoFrame = true;
		while(lookingForVideoFrame)
		{
			int error = av_read_frame(_pFormatContext, &packet);
			switch(error)
			{
			case 0:
				if(packet.stream_index == _videoStream->index)
				{
					_currentFrame = frame;

					readElapsed = _testTimer.elapsed();
					int frameFinished = 0;
					avcodec_decode_video2(_videoStream->codec, _videoFrame, &frameFinished, &packet);
					if(frameFinished)
					{
						decodeElapsed = _testTimer.elapsed();

						int frameHeight = _videoFrame->height;
						if(_settings)
						{
							if(_settings->value("videoDeinterlace", false).toBool())
								frameHeight = _videoFrame->height / 2;
						}
						_pSwsCtx = sws_getCachedContext(_pSwsCtx, _videoFrame->width, _videoStream->codec->height,
														_videoStream->codec->pix_fmt, _videoStream->codec->width, frameHeight,
														AV_PIX_FMT_RGBA, SWS_POINT, NULL, NULL, NULL);

						if(_rgb == NULL)
							_rgb = new uint8_t[_videoFrame->width * frameHeight * 4];
						int linesize = _videoFrame->width * 4;
						if (0 <= sws_scale(_pSwsCtx, (const uint8_t * const *) _videoFrame->data,
										   _videoFrame->linesize, 0, _videoStream->codec->height, &_rgb,
										   &linesize))
						{
							scaleElapsed = _testTimer.elapsed();

							videoRect.createTextureFromARGBBuffer(_rgb, _videoFrame->width, frameHeight);

							textureElapsed = _testTimer.elapsed();

							_videoFrameTickCounter.tick();
							result = true;
						}
						lookingForVideoFrame = false;
					} // if frame decode is not finished, let's read another packet.
				}
				else if(_audioStream && (packet.stream_index == _audioStream->index))
				{
					int ok = 0;
					avcodec_decode_audio4(_audioStream->codec, _audioFrame, &ok, &packet);
					if(ok)
					{
						PHDBG(26) << "audio:" << _audioFrame->nb_samples;
					}
				}
				break;
			case AVERROR_INVALIDDATA:
			case AVERROR_EOF:
			default:
			{
				char errorStr[256];
				av_strerror(error, errorStr, 256);
				PHDEBUG << frame << "error:" << errorStr;
				lookingForVideoFrame = false;
				break;
			}
			}
			//Avoid memory leak
			av_free_packet(&packet);
		}
	}

//	int currentGotoElapsed = _testTimer.elapsed();
	//	if(_testTimer.elapsed() > 25)
	//		PHDEBUG << frame << lastGotoElapsed << seekElapsed - lastGotoElapsed << readElapsed - seekElapsed
	//				<< decodeElapsed - readElapsed << scaleElapsed - decodeElapsed << textureElapsed - scaleElapsed << currentGotoElapsed - lastGotoElapsed << _testTimer.elapsed();
	_testTimer.restart();

	return result;
}

int64_t PhVideoEngine::frame2time(PhFrame f)
{
	int64_t t = 0;
	if(_videoStream)
	{
		PhFrame fps = PhTimeCode::getFps(_clock.timeCodeType());
		t = f * _videoStream->time_base.den / _videoStream->time_base.num / fps;
	}
	return t;
}

PhFrame PhVideoEngine::time2frame(int64_t t)
{
	PhFrame f = 0;
	if(_videoStream)
	{
		PhFrame fps = PhTimeCode::getFps(_clock.timeCodeType());
		f = t * _videoStream->time_base.num * fps / _videoStream->time_base.den;
	}
	return f;
}

PaStreamCallbackResult PhVideoEngine::processAudio(void *outputBuffer, unsigned long samplePerBuffer)
{	
	if(!ready())
	{
		PHDEBUG << "not ready";
		return paAbort;
	}
//	PHDEBUG;
//	memset(outputBuffer, 0, samplePerBuffer * 2);
//	return paContinue;

	AVPacket packet;
	int sampleSize = (_audioStream->codec->channels * _audioStream->codec->bits_per_coded_sample) >> 3;

	int nbSampleRead = 0;
	while(nbSampleRead < samplePerBuffer)
	{
		int error = av_read_frame(_pFormatContext, &packet);
		switch(error)
		{
		case 0:
			if(packet.stream_index == _videoStream->index)
			{
				continue;
				int frameFinished = 0;
				avcodec_decode_video2(_videoStream->codec, _videoFrame, &frameFinished, &packet);
				if(frameFinished)
				{

					int frameHeight = _videoFrame->height;
					if(_settings)
					{
						if(_settings->value("videoDeinterlace", false).toBool())
							frameHeight = _videoFrame->height / 2;
					}
					_pSwsCtx = sws_getCachedContext(_pSwsCtx, _videoFrame->width, _videoStream->codec->height,
													_videoStream->codec->pix_fmt, _videoStream->codec->width, frameHeight,
													AV_PIX_FMT_RGBA, SWS_POINT, NULL, NULL, NULL);

					if(_rgb == NULL)
						_rgb = new uint8_t[_videoFrame->width * frameHeight * 4];
					int linesize = _videoFrame->width * 4;
					if (0 <= sws_scale(_pSwsCtx, (const uint8_t * const *) _videoFrame->data,
									   _videoFrame->linesize, 0, _videoStream->codec->height, &_rgb,
									   &linesize))
					{
						videoRect.createTextureFromARGBBuffer(_rgb, _videoFrame->width, frameHeight);
						_videoFrameTickCounter.tick();
					}
				}
			}
			else if(_audioStream && (packet.stream_index == _audioStream->index))
			{
				int ok = 0;
				avcodec_decode_audio4(_audioStream->codec, _audioFrame, &ok, &packet);
				if(ok)
				{
					PHDBG(0) << "audio:" << _audioFrame->nb_samples << _audioFrame->linesize[0] << sampleSize * nbSampleRead << sampleSize << nbSampleRead;
					memcpy((qint8 *)outputBuffer + sampleSize * nbSampleRead, _audioFrame->data[0], _audioFrame->linesize[0]);
					nbSampleRead += _audioFrame->nb_samples;
					if(nbSampleRead + _audioFrame->nb_samples < samplePerBuffer)
						return paContinue;
				}
			}
			break;
		case AVERROR_INVALIDDATA:
		case AVERROR_EOF:
		default:
		{
			char errorStr[256];
			av_strerror(error, errorStr, 256);
			PHDBG(0) << "Error" << errorStr;
			return paAbort;
		}
		}
		//Avoid memory leak
		av_free_packet(&packet);
	}

		return paContinue;

//	PHDBG(0) << _audioFrame->pkt_size;
//	if(_clock.rate())
//	{
//		memcpy(outputBuffer, _audioFrame->data, _audioFrame->pkt_size);
//	}
//	else
//	{
//		memset(outputBuffer, 0, framesPerBuffer);
//	}


//	return framesPerBuffer;
}

int PhVideoEngine::audioCallback(const void *, void *outputBuffer, unsigned long FPB, const PaStreamCallbackTimeInfo *, PaStreamCallbackFlags , void *userData)
{
	PhVideoEngine * avEngine= (PhVideoEngine *) userData;
	return avEngine->processAudio(outputBuffer, FPB);
}
