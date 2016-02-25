#include <QCoreApplication>

#include "PhTools/PhDebug.h"

#include "PhVideoPool.h"

PhVideoPool::PhVideoPool(PhVideoSettings *settings, PhClock *clock)
	: _settings(settings), _clock(clock),
	_timeIn(0), _length(0), _tcType(PhTimeCodeType25)
{

}

void PhVideoPool::requestFrame(PhTime time)
{
	PhVideoBuffer * buffer;

	if (!_recycledPool.empty()) {
		buffer = _recycledPool.takeFirst();
	}
	else {
		PHDBG(24) << "creating a new buffer";
		buffer = new PhVideoBuffer();
	}

	buffer->setTime(0);
	buffer->setRequestTime(time - _timeIn);

	PHDBG(24) << time - _timeIn;

	// ask the frame to the decoder.
	// Notice that the time origin for the decoder is 0 at the start of the file, it's not timeIn.
	emit decodeFrame(buffer);

	_requestedPool.append(buffer);
}

void PhVideoPool::requestFrames(PhTime time)
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

void PhVideoPool::cancel()
{
	QCoreApplication::processEvents();

	foreach(PhVideoBuffer * requestedFrame, _requestedPool) {
		emit cancelFrameRequest(requestedFrame);
	}
	_cancelledPool.append(_requestedPool);
	_requestedPool.clear();

	_recycledPool.append(_decodedPool);
	_decodedPool.clear();
}

const QList<PhVideoBuffer *> PhVideoPool::decoded()
{
	return _decodedPool;
}

void PhVideoPool::update(PhTime timeIn, PhTime length, PhTimeCodeType tcType)
{
	_timeIn = timeIn;
	_length = length;
	_tcType = tcType;
}

void PhVideoPool::frameAvailable(PhVideoBuffer *buffer)
{
	PHDBG(24) << buffer->requestTime() << buffer->time() << buffer->width() << "x" << buffer->height();
	// move from requested to decoded
	_cancelledPool.removeAll(buffer);
	_requestedPool.removeAll(buffer);
	_decodedPool.append(buffer);

	// cleanup
	cleanup(_clock->time());
}

void PhVideoPool::frameCancelled(PhVideoBuffer *buffer)
{
	if (_cancelledPool.contains(buffer)) {
		_cancelledPool.removeAll(buffer);
		_recycledPool.append(buffer);
	}
}

bool PhVideoPool::isFrameRequested(PhTime time)
{
	// clip to stream boundaries
	if(time < _timeIn)
		time = _timeIn;
	if (time >= _timeIn + _length)
		time = _timeIn + _length;

	QList<PhVideoBuffer *> requestedOrDecodedFrames;
	requestedOrDecodedFrames.append(_requestedPool);
	requestedOrDecodedFrames.append(_decodedPool);

	foreach(PhVideoBuffer *requestedFrame, requestedOrDecodedFrames) {
		PhTime requestedTime = requestedFrame->requestTime() + _timeIn;

		// We consider that we have requested the frame if the time has changed less
		// than the time between two frames.
		if ((time < requestedTime + PhTimeCode::timePerFrame(_tcType))
		    && (time >= requestedTime)) {
			// we have already requested that frame
			return true;
		}
	}

	return false;
}

void PhVideoPool::cleanup(PhTime time)
{
	// clip to stream boundaries
	if(time < _timeIn)
		time = _timeIn;
	if (time >= _timeIn + _length)
		time = _timeIn + _length;

	PhTime halfPoolWindow = _settings->videoPoolWindow();

	QMutableListIterator<PhVideoBuffer*> i(_decodedPool);
	while (i.hasNext()) {
		PhVideoBuffer *buffer = i.next();

		PhTime bufferTime = buffer->time() + _timeIn;

		if ((bufferTime < time - halfPoolWindow) || (bufferTime >= time + halfPoolWindow)) {
			// Given the current playing direction and position,
			// this buffer is not likely to be shown on screen anytime soon.
			i.remove();

			_recycledPool.append(buffer);

			PHDBG(24) << "recycle buffer " << bufferTime - _timeIn;
		}
	}

	QMutableListIterator<PhVideoBuffer*> r(_requestedPool);
	while (r.hasNext()) {
		PhVideoBuffer *buffer = r.next();

		PhTime bufferTime = buffer->requestTime() + _timeIn;

		if ((bufferTime < time - halfPoolWindow) || (bufferTime >= time + halfPoolWindow)) {
			// Given the current playing direction and position,
			// this frame is not likely to be shown on screen anytime soon.
			r.remove();

			_cancelledPool.append(buffer);

			// tell the decoder we no longer want this frame to be decoded
			emit cancelFrameRequest(buffer);

			PHDBG(24) << "cancel frame request " << bufferTime - _timeIn;
		}
	}

	// Make sure we do not have too many recycled frames.
	// They accumulate when seeking, because there is a short time lag
	// between the time when the engine asks to cancel a frame and the
	// time when the decoder really does it.
	while (_recycledPool.count() > 10) {
		PhVideoBuffer * bufferToDelete = _recycledPool.takeFirst();
		delete bufferToDelete;
	}
}

PhVideoBuffer *PhVideoPool::decoded(PhTime time)
{
	// clip to stream boundaries
	if(time < _timeIn)
		time = _timeIn;
	if (time >= _timeIn + _length)
		time = _timeIn + _length;

	foreach(PhVideoBuffer *buffer, _decodedPool) {
		PhTime bufferTime = buffer->time() + _timeIn;

		if ((time < bufferTime + PhTimeCode::timePerFrame(_tcType))
			&& (time >= bufferTime)) {
			return buffer;
		}
	}

	return NULL;
}


