#include <QMessageBox>
#include <QFileDialog>

#include "PhTools/PhDebug.h"
#include "PhCommonUI/PhTimeCodeDialog.h"

#include "GraphicStripSyncTestWindow.h"

GraphicStripSyncTestWindow::GraphicStripSyncTestWindow(GraphicStripSyncTestSettings *settings) :
	GraphicStripTestWindow(settings),
	_settings(settings),
	_sonySlave(PhTimeCodeType25, _settings)
{
	_clockSynchroniser.setStripClock(_clock);

	if(_sonySlave.open()) {
		_clock = _sonySlave.clock();
		_clockSynchroniser.setSonyClock(_clock);
	}
	else
		QMessageBox::critical(this, "Sony Test", "Unable to connect to Sony slave");
}

