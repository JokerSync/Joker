#include "CommonSpec.h"

#include "PhTools/PhDebug.h"
#include "PhVideo/PhVideoDecoder.h"

#include <QSignalSpy>

#define OPEN_WAIT_TIME 10000

using namespace bandit;

go_bandit([](){
	describe("decoder", [](){
		PhVideoDecoder *decoder;
		QSignalSpy *openedSpy, *openFailedSpy, *frameAvailableSpy;

		before_each([&](){
			qRegisterMetaType<PhTime>("PhTime");
			qRegisterMetaType<PhVideoFrame>("PhVideoFrame");

			decoder = new PhVideoDecoder();
			openedSpy = new QSignalSpy(decoder, &PhVideoDecoder::opened);
			openFailedSpy = new QSignalSpy(decoder, &PhVideoDecoder::openFailed);
			frameAvailableSpy = new QSignalSpy(decoder, &PhVideoDecoder::frameAvailable);
		});

		after_each([&](){
			delete openedSpy;
			delete openFailedSpy;
			delete decoder;
		});

		it("succeed", [&](){

			QObject::connect(decoder, &PhVideoDecoder::opened, [&](PhTime length, double framePerSecond, PhTime timeIn, int width, int height, QString codecName) {
				AssertThat(length, Equals(192000));
				AssertThat(framePerSecond, Equals(25.0f));
				AssertThat(timeIn, Equals(0));
				AssertThat(width, Equals(64));
				AssertThat(height, Equals(64));
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
				AssertThat(codecName, Equals("BMP (Windows and OS/2 bitmap)"));
#else
				AssertThat(codecName, Equals("bmp"));
#endif
			});

			AssertThat(openedSpy->count(), Equals(0));
			decoder->open("interlace_%03d.bmp");
			AssertThat(openedSpy->count(), Equals(1));
			AssertThat(openFailedSpy->count(), Equals(0));

		});

		it("fails", [&](){
			decoder->open("unexistingfile.mkv");
			AssertThat(openedSpy->count(), Equals(0));
		});

		it("decode frame", [&](){
			decoder->open("interlace_%03d.bmp");

			PhVideoFrame frame0, frame1, frame2;
			PhVideoFrame *expectedFrame = NULL;
			PhTime expectedTime = 0;

			QObject::connect(decoder, &PhVideoDecoder::frameAvailable, [&](PhVideoFrame *frame){
				AssertThat(frame, Equals(expectedFrame));
				AssertThat(frame->time(), Equals(expectedTime));
			});

			expectedFrame = &frame0;
			frame0.setRequestTime(0);
			decoder->decodeFrame(&frame0);
			AssertThat(frameAvailableSpy->count(), Equals(1));

			expectedFrame = &frame1;
			frame1.setRequestTime(960);
			expectedTime = 960;
			decoder->decodeFrame(&frame1);
			AssertThat(frameAvailableSpy->count(), Equals(2));

			expectedFrame = &frame2;
			frame2.setRequestTime(2000);
			expectedTime = 1920;
			decoder->decodeFrame(&frame2);
			AssertThat(frameAvailableSpy->count(), Equals(3));
		});
	});
});
