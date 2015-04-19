/**
 * @file
 * @copyright (C) 2012-2014 Phonations
 * @license http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include <cmath>

#include "PhTools/PhDebug.h"

#include "PhVideoEngine.h"
#include "PhVideoDecoder.h"

PhVideoEngine::PhVideoEngine(PhVideoSettings *settings) :
	_settings(settings),
	_fileName(""),
	_tcType(PhTimeCodeType25),
	_timeIn(0),
	_formatContext(NULL),
	_videoStream(NULL),
	_currentTime(PHTIMEMIN),
	_useAudio(false),
	_audioStream(NULL),
	_audioFrame(NULL),
	_deinterlace(false)
{
	PHDEBUG << "Using FFMpeg widget for video playback.";
	av_register_all();
	avcodec_register_all();

	// initialize the decoder that operates in a separate thread
	PhVideoDecoder *decoder = new PhVideoDecoder();
	decoder->moveToThread(&_decoderThread);
	connect(&_decoderThread, &QThread::finished, decoder, &QObject::deleteLater);
	connect(this, &PhVideoEngine::decodeFrame, decoder, &PhVideoDecoder::decodeFrame);
	connect(this, &PhVideoEngine::openInDecoder, decoder, &PhVideoDecoder::open);
	connect(this, &PhVideoEngine::closeInDecoder, decoder, &PhVideoDecoder::close);
	connect(decoder, &PhVideoDecoder::frameAvailable, this, &PhVideoEngine::frameAvailable);
	_decoderThread.start();
}

bool PhVideoEngine::ready()
{
	return (_formatContext && _videoStream);
}

void PhVideoEngine::setDeinterlace(bool deinterlace)
{
	PHDEBUG << deinterlace;

	if (deinterlace != _deinterlace) {
		_deinterlace = deinterlace;
		// request the frame again to apply the deinterlace settings
		requestFrame(_currentTime);
	}
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

	// tell the decoder thread to open the file too
	emit openInDecoder(fileName);

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

	PHDEBUG << "length:" << this->length();
	PHDEBUG << "fps:" << this->framePerSecond();
	PHDEBUG << "timebase:" << _videoStream->time_base.num << "/" << _videoStream->time_base.den;

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

	return true;
}

void PhVideoEngine::close()
{
	PHDEBUG << _fileName;

	// tell the decoder thread to close the file too
	emit closeInDecoder();

	// delete all unused buffers
	// Those that are marked as used should not be deleted for now since the decoder thread may be
	// operating on them.
	while (_bufferUsageList.contains(false)) {
		int unusedBufferIndex = _bufferUsageList.indexOf(false);
		delete[] _rgbBufferList.takeAt(unusedBufferIndex);
		_bufferSizeList.removeAt(unusedBufferIndex);
		_bufferUsageList.removeAt(unusedBufferIndex);
	}

	if(_formatContext) {
		PHDEBUG << "Close the media context.";
		if(_videoStream)
			avcodec_close(_videoStream->codec);
		if(_audioStream)
			avcodec_close(_audioStream->codec);
		avformat_close_input(&_formatContext);
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
	PhTime delay = static_cast<PhTime>(_settings->screenDelay() * _clock.rate() * 24.);
	PhTime time = _clock.time() + delay;

	// If the time corresponds to a different frame than the one we currently have,
	// request that frame to the decoder thread by sending a signal.
	// The engine manages the frame buffers.
	if(!isFrameAvailable(time)) {
		requestFrame(time);
	}

	// draw whatever frame we currently have
	if(_settings->useNativeVideoSize())
		_videoRect.setRect(x, y, this->width(), this->height());
	else
		_videoRect.setRect(x, y, w, h);
	_videoRect.setZ(-10);
	_videoRect.draw();
}

void PhVideoEngine::requestFrame(PhTime time)
{
	int frameHeight = _videoStream->codec->height;
	if(_deinterlace)
		frameHeight = frameHeight / 2;
	int bufferSize = avpicture_get_size(AV_PIX_FMT_BGRA, _videoStream->codec->width, frameHeight);

	// find an available buffer
	uint8_t * rgb;
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

	// ask the frame to the decoder.
	// Notice that the time origin for the decoder is 0 at the start of the file, it's not timeIn.
	emit decodeFrame(time - _timeIn, rgb, _deinterlace);

	// update current time so that we do not request the frame again
	_currentTime = time;
}

void PhVideoEngine::setTimeIn(PhTime timeIn)
{
	PHDEBUG << timeIn;
	_timeIn = timeIn;
}

PhTime PhVideoEngine::length()
{
	if(_formatContext)
		return _formatContext->duration * 24000 / AV_TIME_BASE;
	return 0;
}

PhVideoEngine::~PhVideoEngine()
{
	close();

	_decoderThread.quit();
	_decoderThread.wait();

	// the decoder thread has exited, so all the buffers can be cleaned.
	while (!_rgbBufferList.isEmpty())
		delete[] _rgbBufferList.takeFirst();

	_bufferSizeList.clear();
	_bufferUsageList.clear();
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

bool PhVideoEngine::isFrameAvailable(PhTime time)
{
	if(!ready()) {
		PHDEBUG << "not ready";
		return false;
	}

	// clip to stream boundaries
	if(time < _timeIn)
		time = _timeIn;
	if (time >= this->timeOut())
		time = this->timeOut() - tpf;

	if (_currentTime == PHTIMEMIN) {
		return false;
	}

	bool result = false;

	// Stay with the same frame if the time has changed less than the time between two frames
	// Note that av_seek_frame will seek to the _closest_ frame, sometimes a little bit in the "future",
	// so it is necessary to use a little margin for the second comparison, otherwise a seek may
	// be performed on each call to decodeFrame
	if ((time < _currentTime + PhTimeCode::timePerFrame(_tcType))
	    && (time > _currentTime - PhTimeCode::timePerFrame(_tcType)/2)) {
		result = true;
	}

	return result;
}

void PhVideoEngine::frameAvailable(PhTime time, uint8_t *rgb, int width, int height)
{
	// This slot is connected to the decoder thread.
	// We receive here asynchronously the frame freshly decoded.

	// mark that this buffer is now available
	int bufferIndex = _rgbBufferList.indexOf(rgb);
	_bufferUsageList.replace(bufferIndex, false);

	_videoRect.createTextureFromBGRABuffer(rgb, width, height);
	_videoFrameTickCounter.tick();
}

int64_t PhVideoEngine::PhTime_to_AVTimestamp(PhTime time)
{
	int64_t timestamp = 0;
	if(_videoStream) {
		timestamp = time * _videoStream->time_base.den / PHTIMEBASE / _videoStream->time_base.num;
	}
	return timestamp;
}

PhTime PhVideoEngine::AVTimestamp_to_PhTime(int64_t timestamp)
{
	PhTime time = 0;
	if(_videoStream) {
		time = timestamp * PHTIMEBASE * _videoStream->time_base.num / _videoStream->time_base.den;
	}
	return time;
}
