#include "CommonSpec.h"

#include "PhTools/PhDebug.h"
#include "PhVideo/PhVideoDecoder.h"

#include "VideoSpecSettings.h"

using namespace bandit;

go_bandit([](){
	describe("decoder", [](){
		PhVideoDecoder *decoder;
		VideoSpecSettings *settings = new VideoSpecSettings();

		before_each([&](){
			decoder = new PhVideoDecoder(settings);
		});

		it("succeed", [&](){
			int openedCallCount = 0;
			int openFailedCallCount = 0;

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
				openedCallCount += 1;
			});

			QObject::connect(decoder, &PhVideoDecoder::openFailed, [&]() {
				openFailedCallCount += 1;
			});

			AssertThat(openedCallCount, Equals(0));
			decoder->open("interlace_%03d.bmp");
			AssertThat(openedCallCount, Equals(1));
			AssertThat(openFailedCallCount, Equals(0));

		});

		it("fails", [&](){
			int callCount = 0;

			QObject::connect(decoder, &PhVideoDecoder::opened, [&](PhTimeCodeType tcType, PhFrame frameIn, PhFrame length, int width, int height, QString codecName) {
				callCount += 1;
			});

			decoder->open("unexistingfile.mkv");
			AssertThat(callCount, Equals(0));
		});

		it("decode frame", [&](){
			decoder->open("interlace_%03d.bmp");

			QList<PhFrame> decodedFrames;

			QObject::connect(decoder, &PhVideoDecoder::frameAvailable, [&](PhVideoBuffer *buffer){
				AssertThat(buffer->frame(), Is().GreaterThanOrEqualTo(0));
				AssertThat(buffer->height(), Equals(64));
				AssertThat(buffer->width(), Equals(64));
				decodedFrames.append(buffer->frame());
			});

			// decode 5 frames ahead
			decoder->stripTimeChanged(0, false, false);
			AssertThat(decodedFrames.contains(0), IsTrue());
			AssertThat(decodedFrames.contains(1), IsTrue());
			AssertThat(decodedFrames.contains(2), IsTrue());
			AssertThat(decodedFrames.contains(3), IsTrue());
			AssertThat(decodedFrames.contains(4), IsTrue());
			AssertThat(decodedFrames.count(), Equals(5));

			decodedFrames.clear();
			decoder->stripTimeChanged(1, false, false);
			AssertThat(decodedFrames.contains(1), IsTrue());
			AssertThat(decodedFrames.contains(2), IsTrue());
			AssertThat(decodedFrames.contains(3), IsTrue());
			AssertThat(decodedFrames.contains(4), IsTrue());
			AssertThat(decodedFrames.contains(5), IsTrue());
			AssertThat(decodedFrames.count(), Equals(5));

			decodedFrames.clear();
			decoder->stripTimeChanged(2, false, false);
			AssertThat(decodedFrames.contains(2), IsTrue());
			AssertThat(decodedFrames.contains(2), IsTrue());
			AssertThat(decodedFrames.contains(3), IsTrue());
			AssertThat(decodedFrames.contains(4), IsTrue());
			AssertThat(decodedFrames.contains(6), IsTrue());
			AssertThat(decodedFrames.count(), Equals(5));

			// if frame is already in pool, do not decode it again
			decodedFrames.clear();
			decoder->stripTimeChanged(2, false, true);
			AssertThat(decodedFrames.count(), Equals(0));

			// end of file
			decodedFrames.clear();
			decoder->stripTimeChanged(199, false, false);
			AssertThat(decodedFrames.contains(199), IsTrue());
			AssertThat(decodedFrames.count(), Equals(1));
		});
	});
});
