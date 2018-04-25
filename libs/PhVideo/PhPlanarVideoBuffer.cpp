#include "PhPlanarVideoBuffer.h"
#include "PhTools/PhDebug.h"

#include <QThread>

PhPlanarVideoBuffer::PhPlanarVideoBuffer(enum AVPixelFormat pix_format, int width, int height)
	: QAbstractPlanarVideoBuffer(NoHandle)
	, _plane_nb(0)
{
	// find the number of planes from the format
	const AVPixFmtDescriptor *desc = av_pix_fmt_desc_get(pix_format);
	for (int i = 0; i < desc->nb_components; i++)
		_plane_nb = qMax(_plane_nb, desc->comp[i].plane + 1);

	_numBytes = av_image_get_buffer_size(pix_format, width, height, 1);

	// Allocate an image and fill pointers and linesizes accordingly.
	PHDBG(24) << "PhPlanarVideoBuffer alloc" << width << height << pix_format;
	int result = av_image_alloc(_data, _linesizes, width, height, pix_format, 1 /* align */);
	if (result <= 0) {
		PHDEBUG << "Failed to allocate, Joker will crash after 120 seconds (to have time for a dump)";
		QThread::sleep(120);
		abort();
	}
}

void PhPlanarVideoBuffer::setAvFrame(AVFrame *avFrame)
{
	AVPixelFormat pix_format = static_cast<AVPixelFormat>(avFrame->format);
	av_image_copy(_data, _linesizes, (const uint8_t **) avFrame->data, avFrame->linesize, pix_format, avFrame->width, avFrame->height);
}

PhPlanarVideoBuffer::~PhPlanarVideoBuffer()
{
	av_freep(&_data[0]);
}

int PhPlanarVideoBuffer::map(MapMode mode, int *numBytes, int bytesPerLine[4], uchar *data[4])
{
	if (_numBytes == 0) {
		return 0;
	}

	if (numBytes != NULL)
		*numBytes = _numBytes;

	if (bytesPerLine != NULL) {
		for (int i = 0; i < _plane_nb; i++) {
			bytesPerLine[i] = _linesizes[i];
		}
	}

	if (data != NULL) {
		for (int i = 0; i < _plane_nb; i++) {
			data[i] = _data[i];
		}
	}

	return _plane_nb;
}

void PhPlanarVideoBuffer::unmap()
{
	// nothing to do
}

QAbstractVideoBuffer::MapMode PhPlanarVideoBuffer::mapMode() const
{
	return ReadOnly;
}
