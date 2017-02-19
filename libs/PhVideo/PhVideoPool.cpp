#include <QCoreApplication>

#include "PhTools/PhDebug.h"

#include "PhVideoPool.h"

PhVideoPool::PhVideoPool(PhVideoSettings *settings)
	: _settings(settings),
	_frameLength(0),
	_backward(false),
	_stripFrame(PHFRAMEMIN)
{

}

void PhVideoPool::stripTimeChanged(PhFrame stripFrame, bool backward)
{
	if(_frameLength == 0) {
		PHDBG(24) << "not ready";
		return;
	}

	_backward = backward;
	_stripFrame = stripFrame;

	bool isFrameInPool = _decodedPool.contains(stripFrame);
	emit poolTimeChanged(stripFrame, backward, isFrameInPool);
}

void PhVideoPool::cancel()
{
	emit stop();

	foreach(PhVideoBuffer * frame, _decodedPool.values()) {
		emit recycledFrame(frame);
	}

	_decodedPool.clear();
	_stripFrame = PHFRAMEMIN;
}

const QMap<PhFrame, PhVideoBuffer *> PhVideoPool::decoded()
{
	return _decodedPool;
}

void PhVideoPool::update(PhFrame frameLength)
{
	_frameLength = frameLength;
}

void PhVideoPool::frameAvailable(PhVideoBuffer *buffer)
{
	PHDBG(24) << buffer->frame() << buffer->width() << "x" << buffer->height();

	cleanup(_stripFrame);

	// remove any existing buffer at that position
	if (_decodedPool.contains(buffer->frame())) {
		emit recycledFrame(_decodedPool.take(buffer->frame()));
	}

	_decodedPool.insert(buffer->frame(), buffer);
}

void PhVideoPool::cleanup(PhFrame frame)
{
	PhFrame halfPoolSize = _settings->videoPoolSize();

	if (!_decodedPool.empty()) {
		PhVideoBuffer * firstFrame = _decodedPool.first();
		int frameBytes = firstFrame->width()*firstFrame->height()*4;

		// limit to 1 GB of frames
		halfPoolSize = std::min((int)halfPoolSize, 500000000 / frameBytes);
	}

	int cleanupCount = _decodedPool.size() - 2*halfPoolSize;

	if (cleanupCount <= 0) {
		// no need to cleanup
		return;
	}

	frame = clip(frame);

	QMutableMapIterator<PhFrame, PhVideoBuffer *> i(_decodedPool);

	if (!_backward) {
		// QMap.keys is ordered. So, when playing forward, this iterator will work on the older frames first.
		// So it is likely to clean up enough with the very first iterations
		while (i.hasNext() && cleanupCount > 0) {
			i.next();
			PhFrame bufferFrame = i.key();

			if ((bufferFrame < frame - halfPoolSize) || (bufferFrame >= frame + halfPoolSize)) {
				// Given the current playing direction and position,
				// this buffer is not likely to be shown on screen anytime soon.
				PhVideoBuffer *buffer = i.value();
				i.remove();

				PHDBG(24) << "recycle buffer " << bufferFrame;
				emit recycledFrame(buffer);

				cleanupCount--;
			}
		}
	}
	else {
		i.toBack();
		while (i.hasPrevious() && cleanupCount > 0) {
			i.previous();
			PhFrame bufferFrame = i.key();

			if ((bufferFrame < frame - halfPoolSize) || (bufferFrame >= frame + halfPoolSize)) {
				// Given the current playing direction and position,
				// this buffer is not likely to be shown on screen anytime soon.
				PhVideoBuffer *buffer = i.value();
				i.remove();

				PHDBG(24) << "recycle buffer " << bufferFrame;
				emit recycledFrame(buffer);

				cleanupCount--;
			}
		}
	}
}

PhVideoBuffer *PhVideoPool::tryGetFrame(PhFrame frame)
{
	frame = clip(frame);
	return _decodedPool.value(frame, NULL);
}

PhFrame PhVideoPool::clip(PhFrame frame)
{
	// clip to stream boundaries
	if(frame < 0)
		frame = 0;
	if (frame >= _frameLength)
		frame = _frameLength;

	return frame;
}


