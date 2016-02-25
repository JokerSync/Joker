#include <QCoreApplication>

#include "PhTools/PhDebug.h"

#include "PhVideoFramePool.h"

PhVideoFramePool::PhVideoFramePool(PhVideoSettings *settings, PhClock *clock)
	: _settings(settings), _clock(clock),
	_timeIn(0), _length(0), _tcType(PhTimeCodeType25)
{

}

void PhVideoFramePool::requestFrame(PhTime time)
{
	PhVideoFrame * frame;

	if (!_recycledFramePool.empty()) {
		frame = _recycledFramePool.takeFirst();
	}
	else {
		PHDBG(24) << "creating a new frame";
		frame = new PhVideoFrame();
	}

	frame->setTime(0);
	frame->setRequestTime(time - _timeIn);

	PHDBG(24) << time - _timeIn;

	// ask the frame to the decoder.
	// Notice that the time origin for the decoder is 0 at the start of the file, it's not timeIn.
	emit decodeFrame(frame);

	_requestedFramePool.append(frame);
}

void PhVideoFramePool::requestFrames(PhTime time)
{
	PHDBG(24) << time;
	if(_length == 0) {
		PHDBG(24) << "not ready";
		return;
	}

	// clip to stream boundaries
	if(time < _timeIn)
		time = _timeIn;
	if (time >= _timeIn + _length)
		time = _timeIn + _length;

	// we make sure we have requested "readahead_count" frames
	for (int i = 0; i < _settings->videoReadhead(); i++) {
		int factor = i;
		if (_clock->rate() < 0) {
			factor *= -1;
		}

		PhTime requestTime = time + factor * PhTimeCode::timePerFrame(_tcType);

		if (!isFrameRequested(requestTime)) {
			requestFrame(requestTime);
		}
	}
}

void PhVideoFramePool::cancel()
{
	QCoreApplication::processEvents();

	foreach(PhVideoFrame * requestedFrame, _requestedFramePool) {
		emit cancelFrameRequest(requestedFrame);
	}
	_cancelledFramePool.append(_requestedFramePool);
	_requestedFramePool.clear();

	_recycledFramePool.append(_decodedFramePool);
	_decodedFramePool.clear();
}

const QList<PhVideoFrame *> PhVideoFramePool::decodedFramePool()
{
	return _decodedFramePool;
}

void PhVideoFramePool::update(PhTime timeIn, PhTime length, PhTimeCodeType tcType)
{
	_timeIn = timeIn;
	_length = length;
	_tcType = tcType;
}

void PhVideoFramePool::frameAvailable(PhVideoFrame *frame)
{
	PHDBG(24) << frame->requestTime() << frame->time() << frame->width() << "x" << frame->height();
	// move from requested to decoded
	_cancelledFramePool.removeAll(frame);
	_requestedFramePool.removeAll(frame);
	_decodedFramePool.append(frame);

	// cleanup
	cleanup(_clock->time());
}

void PhVideoFramePool::frameCancelled(PhVideoFrame *frame)
{
	if (_cancelledFramePool.contains(frame)) {
		_cancelledFramePool.removeAll(frame);
		_recycledFramePool.append(frame);
	}
}

bool PhVideoFramePool::isFrameRequested(PhTime time)
{
	// clip to stream boundaries
	if(time < _timeIn)
		time = _timeIn;
	if (time >= _timeIn + _length)
		time = _timeIn + _length;

	QList<PhVideoFrame *> requestedOrDecodedFrames;
	requestedOrDecodedFrames.append(_requestedFramePool);
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

void PhVideoFramePool::cleanup(PhTime time)
{
	// clip to stream boundaries
	if(time < _timeIn)
		time = _timeIn;
	if (time >= _timeIn + _length)
		time = _timeIn + _length;

	PhTime halfPoolWindow = _settings->videoPoolWindow();

	QMutableListIterator<PhVideoFrame*> i(_decodedFramePool);
	while (i.hasNext()) {
		PhVideoFrame *frame = i.next();

		PhTime frameTime = frame->time() + _timeIn;

		if ((frameTime < time - halfPoolWindow) || (frameTime >= time + halfPoolWindow)) {
			// Given the current playing direction and position,
			// this frame is not likely to be shown on screen anytime soon.
			i.remove();

			_recycledFramePool.append(frame);

			PHDBG(24) << "recycle frame " << frameTime - _timeIn;
		}
	}

	QMutableListIterator<PhVideoFrame*> r(_requestedFramePool);
	while (r.hasNext()) {
		PhVideoFrame *frame = r.next();

		PhTime frameTime = frame->requestTime() + _timeIn;

		if ((frameTime < time - halfPoolWindow) || (frameTime >= time + halfPoolWindow)) {
			// Given the current playing direction and position,
			// this frame is not likely to be shown on screen anytime soon.
			r.remove();

			_cancelledFramePool.append(frame);

			// tell the decoder we no longer want this frame to be decoded
			emit cancelFrameRequest(frame);

			PHDBG(24) << "cancel frame request " << frameTime - _timeIn;
		}
	}

	// Make sure we do not have too many recycled frames.
	// They accumulate when seeking, because there is a short time lag
	// between the time when the engine asks to cancel a frame and the
	// time when the decoder really does it.
	while (_recycledFramePool.count() > 10) {
		PhVideoFrame * frameToDelete = _recycledFramePool.takeFirst();
		delete frameToDelete;
	}
}

PhVideoFrame *PhVideoFramePool::frame(PhTime time)
{
	// clip to stream boundaries
	if(time < _timeIn)
		time = _timeIn;
	if (time >= _timeIn + _length)
		time = _timeIn + _length;

	foreach(PhVideoFrame *frame, _decodedFramePool) {
		PhTime frameTime = frame->time() + _timeIn;

		if ((time < frameTime + PhTimeCode::timePerFrame(_tcType))
		    && (time >= frameTime)) {
			return frame;
		}
	}

	return NULL;
}


