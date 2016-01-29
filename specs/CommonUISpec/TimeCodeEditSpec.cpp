
#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeEdit.h"

#include "CommonSpec.h"

using namespace bandit;

go_bandit([](){
	describe("timecode_edit_test", []() {
		before_each([&](){
			PhDebug::disable();
		});

		it("has_frame", [&](){
			PhTimeCodeEdit tcEdit;

			AssertThat(tcEdit.frame(), Equals(0));

			tcEdit.setText("00:00:01:00");
			AssertThat(tcEdit.frame(), Equals(25));
		});

		it("set_frame", [&](){
			PhTimeCodeEdit tcEdit;

			AssertThat(tcEdit.text(),Equals("00:00:00:00"));

			tcEdit.setFrame(25, PhTimeCodeType25);
			AssertThat(tcEdit.text(),Equals("00:00:01:00"));

			tcEdit.setFrame(48, PhTimeCodeType24);
			AssertThat(tcEdit.text(),Equals("00:00:02:00"));
		});

		it("get_keyboard_input01", [&](){
			PhTimeCodeEdit tcEdit;

			AssertThat(tcEdit.selectedText(), Equals(""));

			QTest::keyClicks(&tcEdit, "9");
			AssertThat(tcEdit.text(),Equals("00:00:00:09"));
			AssertThat(tcEdit.frame(), Equals(9));
			AssertThat(tcEdit.isTimeCode(), IsTrue());
			AssertThat(tcEdit.selectedText(), Equals("9"));

			QTest::keyClicks(&tcEdit, "1");
			AssertThat(tcEdit.text(),Equals("00:00:00:91"));
			AssertThat(tcEdit.frame(), Equals(0));
			AssertThat(tcEdit.isTimeCode(), IsFalse());
			AssertThat(tcEdit.selectedText(), Equals("91"));

			QTest::keyClicks(&tcEdit, "2");
			AssertThat(tcEdit.text(),Equals("00:00:09:12"));
			AssertThat(tcEdit.frame(), Equals(9 * 25 + 12));
			AssertThat(tcEdit.isTimeCode(), IsTrue());
			AssertThat(tcEdit.selectedText(), Equals("9:12"));

			QTest::keyClick(&tcEdit, Qt::Key_Backspace);
			AssertThat(tcEdit.text(),Equals("00:00:00:91"));
			AssertThat(tcEdit.frame(), Equals(0));
			AssertThat(tcEdit.isTimeCode(), IsFalse());
			AssertThat(tcEdit.selectedText(), Equals("91"));

			QTest::keyClick(&tcEdit, Qt::Key_Enter);
			AssertThat(tcEdit.text(),Equals("00:00:00:91"));
		});

		it("get_keyboard_input02", [&](){
			PhTimeCodeEdit tcEdit;

			AssertThat(tcEdit.text(),Equals("00:00:00:00"));

			QTest::keyClicks(&tcEdit, "1");
			AssertThat(tcEdit.text(),Equals("00:00:00:01"));

			QTest::keyClicks(&tcEdit, "2345678");
			AssertThat(tcEdit.text(),Equals("12:34:56:78"));

			QTest::keyClicks(&tcEdit, "9");
			AssertThat(tcEdit.text(),Equals("23:45:67:89"));
		});

		it("get_bad_keyboard_input", [&](){
			PhTimeCodeEdit tcEdit;

			tcEdit.setFrame(25, PhTimeCodeType25);
			AssertThat(tcEdit.text(),Equals("00:00:01:00"));

			QTest::keyClicks(&tcEdit, "a");
			AssertThat(tcEdit.text(),Equals("00:00:01:00"));
		});

		it("get_mouse_input", [&](){
			PhTimeCodeEdit tcEdit;

			tcEdit.show();

			AssertThat(tcEdit.selectedText(), Equals(""));

			// Hour testing
			// Vertical axis mouse move
			int x = 20;
			QTest::mousePress(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(x, 5));

			//QTest::mouseMove(&tcEdit, QPoint(130, 200)); // It doesn't seems to work use rather:
			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("01:00:00:00"));
			AssertThat(tcEdit.selectedText(), Equals("01"));

			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("00:00:00:00"));
			AssertThat(tcEdit.selectedText(), Equals("00"));

			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x, 6), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("-01:00:00:00"));
			AssertThat(tcEdit.selectedText(), Equals("01"));

			// Vertical and Horizontal axis mouse move, horizontal moves are out
			// of the text limits
			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x + 100, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("00:00:00:00"));

			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x + 200, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("01:00:00:00"));

			QTest::mouseRelease(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(x, 200));

			//Reset
			tcEdit.setText("00:00:00:00");
			// Minutes testing
			// Vertical axis mouse move
			x = 40;
			QTest::mousePress(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(x, 5), 100);

			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("00:01:00:00"));

			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("00:00:00:00"));

			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x, 6), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("-00:01:00:00"));

			// Vertical and Horizontal axis mouse move, horizontal moves are out
			// of the text limits
			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x + 100, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("00:00:00:00"));

			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x + 200, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("00:01:00:00"));

			QTest::mouseRelease(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(x, 200));

			//Reset
			tcEdit.setText("00:00:00:00");
			// Seconds testing
			// Vertical axis mouse move
			x = 60;
			QTest::mousePress(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(x, 5), 100);

			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("00:00:01:00"));

			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("00:00:00:00"));

			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x, 6), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("-00:00:01:00"));

			// Vertical and Horizontal axis mouse move, horizontal moves are out
			// of the text limits
			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x + 100, 5), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("00:00:00:00"));

			QApplication::sendEvent(&tcEdit, new QMouseEvent(QEvent::MouseMove, QPoint(x + 200, 4), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier));
			AssertThat(tcEdit.text(),Equals("00:00:01:00"));

			QTest::mouseRelease(&tcEdit, Qt::LeftButton, Qt::NoModifier, QPoint(x, 200));

#warning /// @todo do more mouse test
		});
	});
});

