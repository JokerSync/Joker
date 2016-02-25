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
	_frameLength(0),
	_timeIn(0),
	_framePerSecond(25.00f),
	_width(0),
	_height(0),
	_codecName(""),
	_ready(false),
	_deinterlace(false),
	_framePool(settings)
{
	// initialize the decoder that operates in a separate thread
	PhVideoDecoder *decoder = new PhVideoDecoder();
	decoder->moveToThread(&_decoderThread);
	connect(&_clock, &PhClock::timeChanged, this, &PhVideoEngine::onTimeChanged);
	connect(&_decoderThread, &QThread::finished, decoder, &QObject::deleteLater);
	connect(&_framePool, &PhVideoPool::decodeFrame, decoder, &PhVideoDecoder::decodeFrame);
	connect(this, &PhVideoEngine::openInDecoder, decoder, &PhVideoDecoder::open);
	connect(this, &PhVideoEngine::closeInDecoder, decoder, &PhVideoDecoder::close);
	connect(&_framePool, &PhVideoPool::cancelFrameRequest, decoder, &PhVideoDecoder::cancelFrameRequest);
	connect(this, &PhVideoEngine::deinterlaceChanged, decoder, &PhVideoDecoder::setDeinterlace);
	connect(decoder, &PhVideoDecoder::frameAvailable, this, &PhVideoEngine::frameAvailable);
	connect(decoder, &PhVideoDecoder::frameAvailable, &_framePool, &PhVideoPool::frameAvailable);
	connect(decoder, &PhVideoDecoder::frameCancelled, &_framePool, &PhVideoPool::frameCancelled);
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
		_videoRect.discard();

		emit deinterlaceChanged(_deinterlace);

		_framePool.cancel();

		// request the frames again to apply the new deinterlace setting
		_framePool.requestFrames(clockFrame(), _clock.rate() < 0);
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
	_videoRect.discard();

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
	_frameLength = 0;
	_width = 0;
	_height = 0;
	_codecName = "";

	// cancel pool
	_framePool.cancel();
	_framePool.update(0);
}

PhTime PhVideoEngine::clockTime()
{
	PhTime delay = static_cast<PhTime>(_settings->screenDelay() * _clock.rate() * 24.);
	return _clock.time() + delay;
}

PhFrame PhVideoEngine::clockFrame()
{
	return (clockTime() - _timeIn) / PhTimeCode::timePerFrame(_tcType);
}

void PhVideoEngine::drawVideo(int x, int y, int w, int h)
{
	PhFrame frame = clockFrame();

	// 4 possibilities
	// 1) the frame is currently on screen
	//		=> nothing to do
	// 2) or the frame is in the frame pool
	//		=> refresh the texture with the frame that is the pool
	// 3) or the frame has never been requested
	//		=> request the frame
	// 4) or the frame has already been requested
	//		=> nothing to do

	if (!isFrameCurrent(frame)) {
		PhVideoBuffer *buffer = _framePool.decoded(frame);

		if (buffer) {
			// The time does not correspond to the frame on screen,
			// but the frame is in the pool,
			// so just show it.
			PHDBG(24) << "frame found in pool:" << buffer->frame();
			showFrame(buffer);
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
}

PhTime PhVideoEngine::length()
{
	return _frameLength * PhTimeCode::timePerFrame(_tcType);
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

bool PhVideoEngine::isFrameCurrent(PhFrame frame)
{
	if(!ready()) {
		PHDBG(24) << "not ready";
		return false;
	}

	if (_videoRect.currentFrame() == PHFRAMEMIN) {
		// never got a frame
		return false;
	}

	// clip to stream boundaries
	if(frame < 0)
		frame = 0;
	if (frame >= _frameLength)
		frame = _frameLength;

	return _videoRect.currentFrame() == frame;
}

const QList<PhVideoBuffer *> PhVideoEngine::decodedFramePool()
{
	return _framePool.decoded();
}

void PhVideoEngine::showFrame(PhVideoBuffer *buffer)
{
	_videoRect.update(buffer);
	_videoFrameTickCounter.tick();
}

void PhVideoEngine::frameAvailable(PhVideoBuffer *buffer)
{
	// This slot is connected to the decoder thread.
	// We receive here asynchronously the frame freshly decoded.

	PhFrame frame = clockFrame();
	PhFrame bufferFrame = buffer->frame();

	emit newFrameDecoded(bufferFrame);

	if (abs(frame - bufferFrame) <= abs(frame - _videoRect.currentFrame())) {
		// this frame is closer to the current time than the frame that is currently displayed,
		// so show it.
		// Note: we do not wait for the exact frame to be available to improve the responsiveness
		// when seeking.
		showFrame(buffer);
		PHDBG(24) << "showing frame " << bufferFrame;
	}
	else {
		PHDBG(24) << "non-current frame " << bufferFrame;
	}
}

void PhVideoEngine::decoderOpened(PhTimeCodeType tcType, PhFrame frameIn, PhFrame frameLength, int width, int height, QString codecName)
{
	_tcType = tcType;
	_timeIn = frameIn * PhTimeCode::timePerFrame(tcType);
	_frameLength = frameLength;
	_width = width;
	_height = height;
	_codecName = codecName;

	// Looking for timecode type
	_tcType = PhTimeCode::computeTimeCodeType(this->framePerSecond());
	emit timeCodeTypeChanged(_tcType);
	_framePool.update(frameLength);

	_ready = true;

	emit opened(true);

	_framePool.requestFrames(clockFrame(), _clock.rate() < 0);
}

void PhVideoEngine::openInDecoderFailed()
{
	emit opened(false);
}

void PhVideoEngine::onTimeChanged(PhTime)
{
	_framePool.requestFrames(clockFrame(), _clock.rate() < 0);
}
