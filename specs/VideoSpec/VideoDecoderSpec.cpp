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

			QObject::connect(decoder, &PhVideoDecoder::opened, [&](PhTimeCodeType tcType, PhFrame frameIn, PhFrame length, int width, int height, QString codecName) {
				AssertThat(tcType, Equals(PhTimeCodeType25));
				AssertThat(frameIn, Equals(0));
				AssertThat(length, Equals(200));
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

			PhVideoBuffer buffer0, buffer1, buffer2;
			PhVideoBuffer *expectedBuffer = NULL;
			PhFrame expectedFrame = 0;

			QObject::connect(decoder, &PhVideoDecoder::frameAvailable, [&](PhVideoBuffer *buffer){
				AssertThat(buffer, Equals(expectedBuffer));
				AssertThat(buffer->frame(), Equals(expectedFrame));
			});

			expectedBuffer = &buffer0;
			buffer0.setRequestFrame(0);
			decoder->requestFrame(&buffer0);
			decoder->decodeFrame();

			AssertThat(frameAvailableSpy->count(), Equals(1));

			expectedBuffer = &buffer1;
			buffer1.setRequestFrame(1);
			expectedFrame = 1;
			decoder->requestFrame(&buffer1);
			decoder->decodeFrame();
			AssertThat(frameAvailableSpy->count(), Equals(2));

			expectedBuffer = &buffer2;
			buffer2.setRequestFrame(2);
			expectedFrame = 2;
			decoder->requestFrame(&buffer2);
			decoder->decodeFrame();
			AssertThat(frameAvailableSpy->count(), Equals(3));
		});
	});
});
