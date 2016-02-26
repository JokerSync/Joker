#include <QCoreApplication>

#include "PhTools/PhDebug.h"

#include "PhVideoPool.h"

PhVideoPool::PhVideoPool(PhVideoSettings *settings)
	: _settings(settings), _frameLength(0)
{

}

void PhVideoPool::requestFrame(PhFrame frame)
{
	PhVideoBuffer * buffer;

	if (!_recycledPool.empty()) {
		buffer = _recycledPool.takeFirst();
	}
	else {
		PHDBG(24) << "creating a new buffer";
		buffer = new PhVideoBuffer();
	}

	buffer->setFrame(0);
	buffer->setRequestFrame(frame);

	PHDBG(24) << frame;

	// ask the frame to the decoder.
	// Notice that the time origin for the decoder is 0 at the start of the file, it's not timeIn.
	emit decodeFrame(buffer);

	_requestedPool.append(buffer);
}

void PhVideoPool::requestFrames(PhFrame frame, bool backward)
{
	PHDBG(24) << frame;
	if(_frameLength == 0) {
		PHDBG(24) << "not ready";
		return;
	}

	// clip to stream boundaries
	if(frame < 0)
		frame = 0;
	if (frame >= _frameLength)
		frame = _frameLength;

	// we make sure we have requested "readahead_count" frames
	for (int i = 0; i < _settings->videoReadhead(); i++) {
		int factor = i;
		if (backward) {
			factor *= -1;
		}

		PhFrame requestedFrame = frame + factor;

		if (!isFrameRequested(requestedFrame)) {
			requestFrame(requestedFrame);
		}
	}

	cleanup(frame);
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

void PhVideoPool::update(PhFrame frameLength)
{
	_frameLength = frameLength;
}

void PhVideoPool::frameAvailable(PhVideoBuffer *buffer)
{
	PHDBG(24) << buffer->requestFrame() << buffer->frame() << buffer->width() << "x" << buffer->height();
	// move from requested to decoded
	_cancelledPool.removeAll(buffer);
	_requestedPool.removeAll(buffer);
	_decodedPool.append(buffer);
}

void PhVideoPool::frameCancelled(PhVideoBuffer *buffer)
{
	if (_cancelledPool.contains(buffer)) {
		_cancelledPool.removeAll(buffer);
		_recycledPool.append(buffer);
	}
}

bool PhVideoPool::isFrameRequested(PhFrame frame)
{
	// clip to stream boundaries
	if(frame < 0)
		frame = 0;
	if (frame >= _frameLength)
		frame = _frameLength;

	QList<PhVideoBuffer *> requestedOrDecodedFrames;
	requestedOrDecodedFrames.append(_requestedPool);
	requestedOrDecodedFrames.append(_decodedPool);

	foreach(PhVideoBuffer *requestedBuffer, requestedOrDecodedFrames) {
		PhFrame requestedFrame = requestedBuffer->requestFrame();

		// We consider that we have requested the frame if the time has changed less
		// than the time between two frames.
		if (frame == requestedFrame) {
			// we have already requested that frame
			return true;
		}
	}

	return false;
}

void PhVideoPool::cleanup(PhFrame frame)
{
	// clip to stream boundaries
	if(frame < 0)
		frame = 0;
	if (frame >= _frameLength)
		frame = _frameLength;

	PhFrame halfPoolSize = _settings->videoPoolSize();

	QMutableListIterator<PhVideoBuffer*> i(_decodedPool);
	while (i.hasNext()) {
		PhVideoBuffer *buffer = i.next();

		PhFrame bufferFrame = buffer->frame();

		if ((bufferFrame < frame - halfPoolSize) || (bufferFrame >= frame + halfPoolSize)) {
			// Given the current playing direction and position,
			// this buffer is not likely to be shown on screen anytime soon.
			i.remove();

			_recycledPool.append(buffer);

			PHDBG(24) << "recycle buffer " << bufferFrame;
		}
	}

	QMutableListIterator<PhVideoBuffer*> r(_requestedPool);
	while (r.hasNext()) {
		PhVideoBuffer *buffer = r.next();

		PhFrame bufferFrame = buffer->requestFrame();

		if ((bufferFrame < frame - halfPoolSize) || (bufferFrame >= frame + halfPoolSize)) {
			// Given the current position,
			// this frame is not likely to be shown on screen anytime soon.
			r.remove();

			_cancelledPool.append(buffer);

			// tell the decoder we no longer want this frame to be decoded
			emit cancelFrameRequest(buffer);

			PHDBG(24) << "cancel frame request " << bufferFrame;
		}
	}

	// Make sure we do not have too many recycled frames.
	// They accumulate when seeking, because there is a short time lag
	// between the time when the engine asks to cancel a frame and the
	// time when the decoder really does it.
	while (_recycledPool.count() > _settings->videoPoolSize()) {
		PhVideoBuffer * bufferToDelete = _recycledPool.takeFirst();
		delete bufferToDelete;
	}
}

PhVideoBuffer *PhVideoPool::decoded(PhFrame frame)
{
	// clip to stream boundaries
	if(frame < 0)
		frame = 0;
	if (frame >= _frameLength)
		frame = _frameLength;

	foreach(PhVideoBuffer *buffer, _decodedPool) {
		if(frame == buffer->frame())
			return buffer;
	}

	return NULL;
}


