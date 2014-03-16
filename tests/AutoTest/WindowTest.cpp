#include <QTest>
#include <QAction>

#include "PhTools/PhDebug.h"

#include "WindowTest.h"

#include "AutoTestSettings.h"
#include "AutoTestWindow.h"

void WindowTest::fullScreenTest01()
{
	AutoTestSettings settings;
	QByteArray emptyArray;
	settings.setWindowGeometry(emptyArray);
	settings.setFullScreen(false);
	AutoTestWindow w(&settings);
	w.show();
	QVERIFY(!w.isFullScreen());
	QVERIFY(!w.fullScreenAction()->isChecked());

	w.toggleFullScreen();
	QVERIFY(w.isFullScreen());
	QVERIFY(settings.fullScreen());
	QVERIFY(w.fullScreenAction()->isChecked());
	w.close();
}

void WindowTest::fullScreenTest02()
{
	AutoTestSettings settings;
	settings.setFullScreen(true);
	AutoTestWindow w(&settings);
	w.show();
	QTest::qWait(900);
	// full screen take one second to trigger
	QVERIFY(!w.isFullScreen());
	QVERIFY(!w.fullScreenAction()->isChecked());
	QTest::qWait(200);
	QVERIFY(w.isFullScreen());
	QVERIFY(w.fullScreenAction()->isChecked());

	w.fullScreenAction()->trigger();
	QVERIFY(!w.isFullScreen());
	QVERIFY(!w.fullScreenAction()->isChecked());

	w.close();
}
