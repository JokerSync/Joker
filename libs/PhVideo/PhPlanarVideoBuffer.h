#ifndef PHPLANARVIDEOBUFFER_H
#define PHPLANARVIDEOBUFFER_H

#include <QAbstractPlanarVideoBuffer>

extern "C" {
#ifndef INT64_C
/** see http://code.google.com/p/ffmpegsource/issues/detail?id=11#c13 */
#define INT64_C(c) (c ## LL)
/** and http://code.google.com/p/ffmpegsource/issues/detail?id=11#c23 */
#define UINT64_C(c) (c ## ULL)
#endif

#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}

class PhPlanarVideoBuffer : public QAbstractPlanarVideoBuffer
{
public:
	PhPlanarVideoBuffer(enum AVPixelFormat pix_format, int width, int height);
	~PhPlanarVideoBuffer();
	void setAvFrame(AVFrame *avFrame);
	MapMode mapMode() const;
	int map(MapMode mode, int *numBytes, int bytesPerLine[], uchar *data[]);
	void unmap();

private:
	uint8_t *_data[4];
	int _linesizes[4];
	int _numBytes;
	int _plane_nb;
};

#endif // PHPLANARVIDEOBUFFER_H
