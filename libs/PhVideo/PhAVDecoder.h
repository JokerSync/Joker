/**
* Copyright (C) 2012-2014 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PHAVDECODER_H
#define PHAVDECODER_H

extern "C" {
#ifndef INT64_C
/** see http://code.google.com/p/ffmpegsource/issues/detail?id=11#c13 */
#define INT64_C(c) (c ## LL)
/** and http://code.google.com/p/ffmpegsource/issues/detail?id=11#c23 */
#define UINT64_C(c) (c ## ULL)
#endif

#include <libavformat/avformat.h>
#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
}


#include <QObject>
#include <QMap>
#include <QSemaphore>
#include <QMutex>

#include "PhGraphic/PhGraphicTexturedRect.h"
#include "PhTools/PhTime.h"
#include "PhTools/PhTimeCode.h"

class PhAVDecoder : public QObject
{
	Q_OBJECT
public:
	explicit PhAVDecoder(QObject *parent = 0);
	bool open(QString fileName);
	void close();

	uint8_t* getBuffer(PhFrame frame);

signals:
	void finished();
	void error(QString err);

public slots:
	void process();
	void quit();
private:

	int64_t frame2time(PhFrame f);
	PhFrame time2frame(int64_t t);

	QSemaphore _framesProcessed;
	QSemaphore _framesFree;
	QMap<PhFrame, uint8_t * > _nextImages;
	QMutex _nextImagesMutex;

	PhFrame _firstFrame;
	PhFrame _currentFrame;


	AVFormatContext * _pFormatContext;
	AVStream *_videoStream;
	AVFrame * _videoFrame;
	struct SwsContext * _pSwsCtx;

	AVStream *_audioStream;
	AVFrame * _audioFrame;

};

#endif // PHAVDECODER_H
