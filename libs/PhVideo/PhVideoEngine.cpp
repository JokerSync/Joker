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
	_currentTime(PHTIMEMIN),
	_deinterlace(false),
	_length(0),
	_width(0),
	_height(0),
	_framePerSecond(25.00f),
	_codecName(""),
	_ready(false)
{
	// initialize the decoder that operates in a separate thread
	PhVideoDecoder *decoder = new PhVideoDecoder();
	decoder->moveToThread(&_decoderThread);
	connect(&_decoderThread, &QThread::finished, decoder, &QObject::deleteLater);
	connect(this, &PhVideoEngine::decodeFrame, decoder, &PhVideoDecoder::decodeFrame);
	connect(this, &PhVideoEngine::openInDecoder, decoder, &PhVideoDecoder::open);
	connect(this, &PhVideoEngine::closeInDecoder, decoder, &PhVideoDecoder::close);
	connect(decoder, &PhVideoDecoder::frameAvailable, this, &PhVideoEngine::frameAvailable);
	connect(decoder, &PhVideoDecoder::opened, this, &PhVideoEngine::decoderOpened);
	connect(decoder, &PhVideoDecoder::openFailed, this, &PhVideoEngine::openInDecoderFailed);
	_decoderThread.start();
}

bool PhVideoEngine::ready()
{
	return _ready;
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

	PHDEBUG << _fileName << "closed";

	_timeIn = 0;
	_fileName = "";
	_length = 0;
	_width = 0;
	_height = 0;
	_codecName = "";
	_ready = false;
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
	if(!ready()) {
		return;
	}

	int bufferSize = avpicture_get_size(AV_PIX_FMT_BGRA, width(), height());

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
	return _length;
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
	return _width;
}

int PhVideoEngine::height()
{
	return _height;
}

double PhVideoEngine::framePerSecond()
{
	return _framePerSecond;
}

QString PhVideoEngine::codecName()
{
	return _codecName;
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

void PhVideoEngine::decoderOpened(PhTime length, double framePerSecond, PhTime timeIn, int width, int height, QString codecName)
{
	_length = length;
	_framePerSecond = framePerSecond;
	_timeIn = timeIn;
	_width = width;
	_height = height;
	_codecName = codecName;

	PHDEBUG << timeIn;

	// Looking for timecode type
	_tcType = PhTimeCode::computeTimeCodeType(this->framePerSecond());
	emit timeCodeTypeChanged(_tcType);

	_ready = true;

	emit opened(true);
}

void PhVideoEngine::openInDecoderFailed()
{
	emit opened(false);
}
