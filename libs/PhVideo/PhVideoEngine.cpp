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
	_deinterlace(false)
{
	// initialize the decoder that operates in a separate thread
	PhVideoDecoder *decoder = new PhVideoDecoder();
	decoder->moveToThread(&_decoderThread);
	connect(&_decoderThread, &QThread::finished, decoder, &QObject::deleteLater);
	connect(this, &PhVideoEngine::decodeFrame, decoder, &PhVideoDecoder::decodeFrame);
	connect(this, &PhVideoEngine::openInDecoder, decoder, &PhVideoDecoder::open);
	connect(this, &PhVideoEngine::closeInDecoder, decoder, &PhVideoDecoder::close);
	connect(this, &PhVideoEngine::cancelFrameRequest, decoder, &PhVideoDecoder::cancelFrameRequest);
	connect(this, &PhVideoEngine::deinterlaceChanged, decoder, &PhVideoDecoder::setDeinterlace);
	connect(decoder, &PhVideoDecoder::frameAvailable, this, &PhVideoEngine::frameAvailable);
	connect(decoder, &PhVideoDecoder::frameCancelled, this, &PhVideoEngine::frameCancelled);
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

		emit deinterlaceChanged(_deinterlace);

		foreach(PhVideoFrame * requestedFrame, _requestedframePool) {
			emit cancelFrameRequest(requestedFrame);
		}
		_cancelledframePool.append(_requestedframePool);
		_requestedframePool.clear();

		_recycledframePool.append(_decodedFramePool);
		_decodedFramePool.clear();

		// request the frames again to apply the new deinterlace setting
		requestFrames(clockTime());
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

	foreach(PhVideoFrame * requestedFrame, _requestedframePool) {
		emit cancelFrameRequest(requestedFrame);
	}
	_cancelledframePool.append(_requestedframePool);
	_requestedframePool.clear();

	_recycledframePool.append(_decodedFramePool);
	_decodedFramePool.clear();

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
	_ready = false;

	foreach(PhVideoFrame * requestedFrame, _requestedframePool) {
		emit cancelFrameRequest(requestedFrame);
	}
	_cancelledframePool.append(_requestedframePool);
	_requestedframePool.clear();

	_recycledframePool.append(_decodedFramePool);
	_decodedFramePool.clear();
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
	time = _timeIn + ((time - _timeIn)/tpf)*tpf;

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
		PhVideoFrame *frame = frameFromPool(time);

		if (frame) {
			// The time does not correspond to the frame on screen,
			// but the frame is in the pool,
			// so just show it.
			PHDEBUG << "frame found in pool";
			showFrame(frame);
		}
	}

	requestFrames(time);

	// draw whatever frame we currently have
	if(_settings->useNativeVideoSize())
		_videoRect.setRect(x, y, this->width(), this->height());
	else
		_videoRect.setRect(x, y, w, h);
	_videoRect.setZ(-10);
	_videoRect.draw();
}

void PhVideoEngine::requestFrames(PhTime time)
{
	const int readahead_count = 5;

	bool playingForward = true;
	if (_clock.rate() < 0) {
		playingForward = false;
	}

	// we make sure we have requested "readahead_count" frames
	for (int i = 0; i < readahead_count; i++) {
		int factor = i;
		if (!playingForward) {
			factor *= -1;
		}

		PhTime requestTime = time + factor*PhTimeCode::timePerFrame(_tcType);

		if (!isFrameRequested(requestTime)) {
			requestFrame(requestTime);
		}
	}
}

void PhVideoEngine::requestFrame(PhTime time)
{
	if(!ready()) {
		return;
	}

	PhVideoFrame * frame;

	if (!_recycledframePool.empty()) {
		frame = _recycledframePool.takeFirst();
	}
	else {
		PHDEBUG << "creating a new frame";
		frame = new PhVideoFrame(0);
	}

	frame->setTime(0);
	frame->setRequestTime(time - _timeIn);

	PHDEBUG << "request frame " << time - _timeIn;

	// ask the frame to the decoder.
	// Notice that the time origin for the decoder is 0 at the start of the file, it's not timeIn.
	emit decodeFrame(frame);

	_requestedframePool.append(frame);
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
		PHDEBUG << "not ready";
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

bool PhVideoEngine::isFrameRequested(PhTime time)
{
	if(!ready()) {
		PHDEBUG << "not ready";
		return false;
	}

	// clip to stream boundaries
	if(time < _timeIn)
		time = _timeIn;
	if (time >= this->timeOut())
		time = this->timeOut();

	QList<PhVideoFrame *> requestedOrDecodedFrames;
	requestedOrDecodedFrames.append(_requestedframePool);
	requestedOrDecodedFrames.append(_decodedFramePool);

	foreach(PhVideoFrame *requestedFrame, requestedOrDecodedFrames) {
		PhTime requestedTime = requestedFrame->requestTime() + _timeIn;

		// We consider that we have requested the frame if the time has changed less
		// than the time between two frames.
		if ((time < requestedTime + PhTimeCode::timePerFrame(_tcType))
		    && (time >= requestedTime)) {
			// we have already requested	that frame
			return true;
		}
	}

	return false;
}

void PhVideoEngine::cleanupFramePools()
{
	PhTime time = clockTime();

	bool playingForward = true;
	if (_clock.rate() < 0) {
		playingForward = false;
	}

	QMutableListIterator<PhVideoFrame*> i(_decodedFramePool);
	while (i.hasNext()) {
		PhVideoFrame *frame = i.next();

		PhTime frameTime = frame->time() + _timeIn;

		if ((playingForward && (frameTime < time - 2*PhTimeCode::timePerFrame(_tcType)
		                        || frameTime >= time + 5*PhTimeCode::timePerFrame(_tcType)))
		    || (!playingForward && (frameTime >= time + 2*PhTimeCode::timePerFrame(_tcType)
		                            || frameTime < time - 5*PhTimeCode::timePerFrame(_tcType)))
		    ) {
			// Given the current playing direction and position,
			// this frame is not likely to be shown on screen anytime soon.
			i.remove();

			_recycledframePool.append(frame);

			PHDEBUG << "recycle frame " << frameTime - _timeIn;
		}
	}

	QMutableListIterator<PhVideoFrame*> r(_requestedframePool);
	while (r.hasNext()) {
		PhVideoFrame *frame = r.next();

		PhTime frameTime = frame->requestTime() + _timeIn;

		if ((playingForward && (frameTime < time - 2*PhTimeCode::timePerFrame(_tcType)
		                        || frameTime >= time + 5*PhTimeCode::timePerFrame(_tcType)))
		    || (!playingForward && (frameTime >= time + 2*PhTimeCode::timePerFrame(_tcType)
		                            || frameTime < time - 5*PhTimeCode::timePerFrame(_tcType)))
		    ) {
			// Given the current playing direction and position,
			// this frame is not likely to be shown on screen anytime soon.
			r.remove();

			_cancelledframePool.append(frame);

			// tell the decoder we no longer want this frame to be decoded
			emit cancelFrameRequest(frame);

			PHDEBUG << "cancel frame request " << frameTime - _timeIn;
		}
	}

	// Make sure we do not have too many recycled frames.
	// They accumulate when seeking, because there is a short time lag
	// between the time when the engine asks to cancel a frame and the
	// time when the decoder really does it.
	while (_recycledframePool.count() > 10) {
		PhVideoFrame * frameToDelete = _recycledframePool.takeFirst();
		delete frameToDelete;
	}
}

PhVideoFrame* PhVideoEngine::frameFromPool(PhTime time)
{
	foreach(PhVideoFrame *frame, _decodedFramePool) {
		PhTime frameTime = frame->time() + _timeIn;

		if ((time < frameTime + PhTimeCode::timePerFrame(_tcType))
		    && (time >= frameTime)) {
			return frame;
		}
	}

	return NULL;
}

void PhVideoEngine::showFrame(PhVideoFrame *frame)
{
	_videoRect.createTextureFromBGRABuffer(frame->rgb(), frame->width(), frame->height());
	_videoFrameTickCounter.tick();

	// update the current time with the true frame time as sent by the decoder
	_currentFrameTime = frame->time() + _timeIn;

	// this signal is used for tests, where some form of synchronization is needed
	emit newFrameDisplayed(_currentFrameTime);
}

void PhVideoEngine::frameAvailable(PhVideoFrame *frame)
{
	// This slot is connected to the decoder thread.
	// We receive here asynchronously the frame freshly decoded.

	PhTime time = clockTime();
	PhTime frameTime = frame->time() + _timeIn;

	if (abs(time - frameTime) <= abs(time - _currentFrameTime)) {
		// this frame is closer to the current time than the frame that is currently displayed,
		// so show it.
		// Note: we do not wait for the exact frame to be available to improve the responsiveness
		// when seeking.
		showFrame(frame);
		PHDEBUG << "showing frame " << frameTime - _timeIn << " " << time - _timeIn;
	}
	else {
		PHDEBUG << "non-current frame " << frameTime - _timeIn << " " << time - _timeIn;
	}

	// move from requested to decoded
	_cancelledframePool.removeAll(frame);
	_requestedframePool.removeAll(frame);
	_decodedFramePool.append(frame);

	// cleanup
	cleanupFramePools();
}

void PhVideoEngine::frameCancelled(PhVideoFrame *frame)
{
	if (_cancelledframePool.contains(frame)) {
		_cancelledframePool.removeAll(frame);
		_recycledframePool.append(frame);
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

	_ready = true;

	emit opened(true);
}

void PhVideoEngine::openInDecoderFailed()
{
	emit opened(false);
}
