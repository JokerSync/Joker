/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhVideoEngine.h"

PhVideoEngine::PhVideoEngine(bool useAudio, QObject *parent) :  QObject(parent),
	_settings(NULL),
	_fileName(""),
	_clock(PhTimeCodeType25),
	_firstFrame(0)
{
	PHDEBUG << "Using FFMpeg widget for video playback.";
	av_register_all();
	_framesProcessed = new QSemaphore(0);
	_framesFree = new QSemaphore(100);
	_decoder = NULL;
	_videoRect = new PhGraphicTexturedRect();
	_nextImages = new QMap<PhFrame, uint8_t * >;
}

bool PhVideoEngine::ready()
{
//	return (_pFormatContext && _videoStream && _videoFrame);
	return true;
}

bool PhVideoEngine::open(QString fileName)
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

	// Find video stream :
	for(int i = 0; i < (int)_pFormatContext->nb_streams; i++) {
		AVMediaType streamType = _pFormatContext->streams[i]->codec->codec_type;
		PHDEBUG << i << ":" << streamType;
		switch(streamType) {
		case AVMEDIA_TYPE_VIDEO:
			_videoStream = _pFormatContext->streams[i];
			PHDEBUG << "\t=> video";
			break;
		case AVMEDIA_TYPE_AUDIO:
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
		_firstFrame = PhTimeCode::frameFromString(tag->value, _clock.timeCodeType());
	}

	// Looking for timecode type
	float fps = this->framePerSecond();
	if(fps == 0) {
		PHDEBUG << "Bad fps detect => assuming 25";
		_clock.setTimeCodeType(PhTimeCodeType25);
	}
	else if(fps < 24)
		_clock.setTimeCodeType(PhTimeCodeType2398);
	else if (fps < 24.5f)
		_clock.setTimeCodeType(PhTimeCodeType24);
	else if (fps < 26)
		_clock.setTimeCodeType(PhTimeCodeType25);
	else if (fps < 30)
		_clock.setTimeCodeType(PhTimeCodeType2997);
	else {
#warning /// @todo patch for #107 => find better fps decoding
		PHDEBUG << "Bad fps detect => assuming 25";
		_clock.setTimeCodeType(PhTimeCodeType25);
	}

	PHDEBUG << "size : " << _videoStream->codec->width << "x" << _videoStream->codec->height;
	AVCodec * videoCodec = avcodec_find_decoder(_videoStream->codec->codec_id);
	if(videoCodec == NULL) {
		PHDEBUG << "Unable to find the codec:" << _videoStream->codec->codec_id;
		return false;
	}


	PHDEBUG << "length:" << this->length();
	PHDEBUG << "fps:" << this->framePerSecond();
	_clock.setFrame(0);

	_decoder = new PhAVDecoder(_framesProcessed, _framesFree, * _nextImages);
	PHDEBUG << &_nextImages;
	_decoder->open(fileName);

	_thread = new QThread;

	_decoder->moveToThread(_thread);
	connect(_decoder, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
	connect(_thread, SIGNAL(started()), _decoder, SLOT(process()));
	connect(_decoder, SIGNAL(finished()), _thread, SLOT(quit()));
	connect(_decoder, SIGNAL(finished()), _decoder, SLOT(deleteLater()));
	connect(_thread, SIGNAL(finished()), _thread, SLOT(deleteLater()));
	_thread->start();
	_fileName = fileName;

	return true;
}

void PhVideoEngine::close()
{
	_fileName = "";
}

void PhVideoEngine::setSettings(PhVideoSettings *settings)
{
	_settings = settings;
}

void PhVideoEngine::drawVideo(int x, int y, int w, int h)
{
	//	_clock.tick(60);
	PhFrame delay = 0;
	if(_settings)
		delay = _settings->screenDelay() * PhTimeCode::getFps(_clock.timeCodeType()) * _clock.rate() / 1000;

	if(_decoder)
	{
		if((_oldFrame - (_clock.frame() + delay) != 0) && _framesProcessed->tryAcquire())
		{
			PHDEBUG << "Read :" << _clock.frame() + delay << _nextImages[(_clock.frame() + delay)];
			_videoRect->createTextureFromARGBBuffer(&_nextImages[(_clock.frame() + delay)], _videoStream->codec->width, _videoStream->codec->height);
			_nextImages->remove((_clock.frame() + delay));
			_framesFree->release();
			_oldFrame = _clock.frame() + delay;
		}
		_videoRect->setRect(x, y, w, h);
		_videoRect->setZ(-10);
		_videoRect->draw();
	}
}

void PhVideoEngine::errorString(QString msg)
{
	PHDEBUG << msg;
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
	if(_videoStream) {
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

int64_t PhVideoEngine::frame2time(PhFrame f)
{
	int64_t t = 0;
	if(_videoStream) {
		PhFrame fps = PhTimeCode::getFps(_clock.timeCodeType());
		t = f * _videoStream->time_base.den / _videoStream->time_base.num / fps;
	}
	return t;
}

PhFrame PhVideoEngine::time2frame(int64_t t)
{
	PhFrame f = 0;
	if(_videoStream) {
		PhFrame fps = PhTimeCode::getFps(_clock.timeCodeType());
		f = t * _videoStream->time_base.num * fps / _videoStream->time_base.den;
	}
	return f;
}
