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
	_length(0),
	_timeIn(0),
	_framePerSecond(25.00f),
	_width(0),
	_height(0),
	_codecName(""),
	_ready(false),
	_currentFrameTime(PHTIMEMIN),
	_deinterlace(false),
	_framePool(settings, &_clock)
{
	// initialize the decoder that operates in a separate thread
	PhVideoDecoder *decoder = new PhVideoDecoder();
	decoder->moveToThread(&_decoderThread);
	connect(&_clock, &PhClock::timeChanged, &_framePool, &PhVideoFramePool::requestFrames);
	connect(&_decoderThread, &QThread::finished, decoder, &QObject::deleteLater);
	connect(&_framePool, &PhVideoFramePool::decodeFrame, decoder, &PhVideoDecoder::decodeFrame);
	connect(this, &PhVideoEngine::openInDecoder, decoder, &PhVideoDecoder::open);
	connect(this, &PhVideoEngine::closeInDecoder, decoder, &PhVideoDecoder::close);
	connect(&_framePool, &PhVideoFramePool::cancelFrameRequest, decoder, &PhVideoDecoder::cancelFrameRequest);
	connect(this, &PhVideoEngine::deinterlaceChanged, decoder, &PhVideoDecoder::setDeinterlace);
	connect(decoder, &PhVideoDecoder::frameAvailable, this, &PhVideoEngine::frameAvailable);
	connect(decoder, &PhVideoDecoder::frameAvailable, &_framePool, &PhVideoFramePool::frameAvailable);
	connect(decoder, &PhVideoDecoder::frameCancelled, &_framePool, &PhVideoFramePool::frameCancelled);
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
		_currentFrameTime = PHTIMEMIN;

		emit deinterlaceChanged(_deinterlace);

		_framePool.cancel();

		// request the frames again to apply the new deinterlace setting
		_framePool.requestFrames(clockTime());
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
	_currentFrameTime = PHTIMEMIN;

	// cancel pool
	_framePool.cancel();

	_fileName = fileName;

	return true;
}

void PhVideoEngine::close()
{
	PHDEBUG << _fileName;

	// tell the decoder thread to close the file too
	emit closeInDecoder();

	PHDEBUG << _fileName << "closed";

	_timeIn = 0;
	_fileName = "";
	_length = 0;
	_width = 0;
	_height = 0;
	_codecName = "";

	// cancel pool
	_framePool.cancel();
	_framePool.update(0, 0, PhTimeCodeType25);
}

PhTime PhVideoEngine::clockTime()
{
	PhTime delay = static_cast<PhTime>(_settings->screenDelay() * _clock.rate() * 24.);
	return _clock.time() + delay;
}

void PhVideoEngine::drawVideo(int x, int y, int w, int h)
{
	PhTime time = clockTime();

	// Round the time to multiple of timePerFrame
	// This avoids issues with time comparisons.
	PhTime tpf = PhTimeCode::timePerFrame(_tcType);
	time = _timeIn + ((time - _timeIn) / tpf) * tpf;

	// 4 possibilities
	// 1) the frame is currently on screen
	//		=> nothing to do
	// 2) or the frame is in the frame pool
	//		=> refresh the texture with the frame that is the pool
	// 3) or the frame has never been requested
	//		=> request the frame
	// 4) or the frame has already been requested
	//		=> nothing to do

	if (!isFrameCurrent(time)) {
		PhVideoFrame *frame = _framePool.frame(time);

		if (frame) {
			// The time does not correspond to the frame on screen,
			// but the frame is in the pool,
			// so just show it.
			PHDBG(24) << "frame found in pool:" << frame->time();
			showFrame(frame);
		}
	}

	// draw whatever frame we currently have
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
	_framePool.update(_timeIn, _length, _tcType);
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

bool PhVideoEngine::isFrameCurrent(PhTime time)
{
	if(!ready()) {
		PHDBG(24) << "not ready";
		return false;
	}

	// clip to stream boundaries
	if(time < _timeIn)
		time = _timeIn;
	if (time >= this->timeOut())
		time = this->timeOut() - tpf;

	if (_currentFrameTime == PHTIMEMIN) {
		// never got a frame
		return false;
	}

	bool result = false;

	// Stay with the same frame if the time has changed less than the time between two frames
	if ((time < _currentFrameTime + PhTimeCode::timePerFrame(_tcType))
	    && (time >= _currentFrameTime)) {
		// we have already that frame
		result = true;
	}

	return result;
}

const QList<PhVideoFrame *> PhVideoEngine::decodedFramePool()
{
	return _framePool.decodedFramePool();
}

void PhVideoEngine::showFrame(PhVideoFrame *frame)
{
	_videoRect.createTextureFromBGRABuffer(frame->rgb(), frame->width(), frame->height());
	_videoFrameTickCounter.tick();

	// update the current time with the true frame time as sent by the decoder
	_currentFrameTime = frame->time() + _timeIn;
}

void PhVideoEngine::frameAvailable(PhVideoFrame *frame)
{
	// This slot is connected to the decoder thread.
	// We receive here asynchronously the frame freshly decoded.

	PhTime time = clockTime();
	PhTime frameTime = frame->time() + _timeIn;

	emit newFrameDecoded(frameTime);

	if (abs(time - frameTime) <= abs(time - _currentFrameTime)) {
		// this frame is closer to the current time than the frame that is currently displayed,
		// so show it.
		// Note: we do not wait for the exact frame to be available to improve the responsiveness
		// when seeking.
		showFrame(frame);
		PHDBG(24) << "showing frame " << frameTime;
	}
	else {
		PHDBG(24) << "non-current frame " << frameTime;
	}
}

void PhVideoEngine::decoderOpened(PhTime length, double framePerSecond, PhTime timeIn, int width, int height, QString codecName)
{
	_length = length;
	_framePerSecond = framePerSecond;
	_timeIn = timeIn;
	_width = width;
	_height = height;
	_codecName = codecName;

	// Looking for timecode type
	_tcType = PhTimeCode::computeTimeCodeType(this->framePerSecond());
	emit timeCodeTypeChanged(_tcType);
	_framePool.update(timeIn, length, _tcType);

	_ready = true;

	emit opened(true);

	_framePool.requestFrames(_clock.time());
}

void PhVideoEngine::openInDecoderFailed()
{
	emit opened(false);
}
