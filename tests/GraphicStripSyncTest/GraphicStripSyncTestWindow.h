#ifndef GRAPHICSTRIPSYNCTESTWINDOW_H
#define GRAPHICSTRIPSYNCTESTWINDOW_H

#include "PhSync/PhSonySlaveController.h"

#include "../GraphicStripTest/GraphicStripTestWindow.h"

#include "GraphicStripSyncTestSettings.h"
#include "SonyStripSynchronizer.h"

class GraphicStripSyncTestWindow : public GraphicStripTestWindow
{
	Q_OBJECT

public:
	GraphicStripSyncTestWindow(GraphicStripSyncTestSettings *settings);

private:
	GraphicStripSyncTestSettings *_settings;
	PhSonySlaveController _sonySlave;
	SonyStripSynchronizer _clockSynchroniser;
};

#endif // GRAPHICSTRIPSYNCTESTWINDOW_H
