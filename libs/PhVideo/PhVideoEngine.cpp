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
	_bilinearFiltering(true),
	_deinterlace(false),
	_framePool(settings)
{
	// initialize the decoder that operates in a separate thread
	PhVideoDecoder *decoder = new PhVideoDecoder(settings);
	decoder->moveToThread(&_decoderThread);
	connect(&_clock, &PhClock::timeChanged, this, &PhVideoEngine::onTimeChanged);
	connect(this, &PhVideoEngine::stripTimeChanged, &_framePool, &PhVideoPool::stripTimeChanged);
	connect(&_decoderThread, &QThread::finished, decoder, &QObject::deleteLater);
	connect(this, &PhVideoEngine::openInDecoder, decoder, &PhVideoDecoder::open);
	connect(this, &PhVideoEngine::closeInDecoder, decoder, &PhVideoDecoder::close);
	connect(&_framePool, &PhVideoPool::recycledFrame, decoder, &PhVideoDecoder::recycleFrame);
	connect(&_framePool, &PhVideoPool::stop, decoder, &PhVideoDecoder::stop);
	connect(&_framePool, &PhVideoPool::poolTimeChanged, decoder, &PhVideoDecoder::stripTimeChanged);
	connect(this, &PhVideoEngine::deinterlaceChanged, decoder, &PhVideoDecoder::setDeinterlace);
	connect(decoder, &PhVideoDecoder::frameAvailable, this, &PhVideoEngine::frameAvailable);
	connect(decoder, &PhVideoDecoder::frameAvailable, &_framePool, &PhVideoPool::frameAvailable);
	connect(decoder, &PhVideoDecoder::opened, this, &PhVideoEngine::decoderOpened);
	connect(decoder, &PhVideoDecoder::openFailed, this, &PhVideoEngine::openInDecoderFailed);

	// decoder has lower priority that the UI
	_decoderThread.start(QThread::LowPriority);
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
		foreach (PhVideoRect *videoRect, _videoRectList.values()) {
			videoRect->discard();
		}

		emit deinterlaceChanged(_deinterlace);

		_framePool.cancel();

		// request the frames again to apply the new deinterlace setting
		emit stripTimeChanged(clockFrame(), _clock.rate() < 0);
	}
}

bool PhVideoEngine::bilinearFiltering()
{
	return _bilinearFiltering;
}

void PhVideoEngine::setBilinearFiltering(bool bilinear)
{
	_bilinearFiltering = bilinear;
	foreach (PhVideoRect *videoRect, _videoRectList.values()) {
		videoRect->setBilinearFiltering(bilinear);
	}
}

bool PhVideoEngine::open(QString fileName)
{
	close();

	PHDEBUG << fileName;

	// tell the decoder thread to open the file too
	emit openInDecoder(fileName);

	_clock.setTime(0);
	_clock.setRate(0);
	foreach (PhVideoRect *videoRect, _videoRectList.values()) {
		videoRect->discard();
	}

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

PhFrame PhVideoEngine::clockFrame()
{
	return (_clock.time() - _timeIn) / PhTimeCode::timePerFrame(_tcType);
}

void PhVideoEngine::drawVideo(int x, int y, int w, int h, PhTime offset)
{
	PhFrame frameOffset = offset / PhTimeCode::timePerFrame(_tcType);
	PhFrame frame = clockFrame() + frameOffset;

	// clip to stream boundaries
	if(frame < 0)
		frame = 0;
	if (frame >= _frameLength)
		frame = _frameLength;

	// 2 possibilities
	// 1) the frame is currently on screen
	//		=> nothing to do
	// 2) or the frame is in the frame pool
	//		=> refresh the texture with the frame that is the pool
	// in any case, transmit the current time to the decoder so that it can read ahead

	PhVideoRect *videoRect = _videoRectList[frameOffset];
	if(videoRect == NULL) {
		_videoRectList[frameOffset] = videoRect = new PhVideoRect();
		videoRect->setBilinearFiltering(_bilinearFiltering);
	}
	if (videoRect->currentFrame() != frame) {
		PhVideoBuffer *buffer = _framePool.tryGetFrame(frame);

		if (buffer) {
			// The time does not correspond to the frame on screen,
			// but the frame is in the pool,
			// so just show it.
			PHDBG(24) << "frame found in pool:" << buffer->frame();
			videoRect->update(buffer);
			_videoFrameTickCounter.tick();
		}
	}

	// draw whatever frame we currently have
	if(_settings->useNativeVideoSize())
		videoRect->setRect(x, y, this->width(), this->height());
	else
		videoRect->setRect(x, y, w, h);
	videoRect->setZ(-10);
	videoRect->draw();
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

	emit stopDecoder();
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

const QMap<PhFrame, PhVideoBuffer *> PhVideoEngine::decodedFramePool()
{
	return _framePool.decoded();
}

void PhVideoEngine::frameAvailable(PhVideoBuffer *buffer)
{
	// This slot is connected to the decoder thread.
	// We receive here asynchronously the frame freshly decoded.

	PhFrame frame = clockFrame();
	PhFrame bufferFrame = buffer->frame();

	emit newFrameDecoded(bufferFrame);
	bool shown = false;
	foreach (PhFrame offset, _videoRectList.keys()) {
		if ((abs(frame + offset - bufferFrame) <= abs(frame + offset - _videoRectList[offset]->currentFrame())
		     && abs(frame + offset - bufferFrame) < 3)
		    || abs(frame + offset - bufferFrame) + 50 <= abs(frame + offset - _videoRectList[offset]->currentFrame())) {
			// this frame is closer to the current time than the frame that is currently displayed,
			// so show it.
			// Note: we do not wait for the exact frame to be available to improve the responsiveness
			// when seeking.
			_videoRectList[offset]->update(buffer);
			_videoFrameTickCounter.tick();
			PHDBG(24) << "showing frame " << bufferFrame;
			shown = true;
		}
	}
	if (!shown) {
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

	emit stripTimeChanged(clockFrame(), _clock.rate() < 0);
}

void PhVideoEngine::openInDecoderFailed()
{
	emit opened(false);
}

void PhVideoEngine::onTimeChanged(PhTime)
{
	PHDBG(24) << clockFrame() << " " << (_clock.rate() < 0);
	emit stripTimeChanged(clockFrame(), _clock.rate() < 0);
}
