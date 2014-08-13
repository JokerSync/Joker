#ifndef GRAPHICSYNCTESTWINDOW_H
#define GRAPHICSYNCTESTWINDOW_H

#include <QMainWindow>
#include <QTime>

#include "PhSony/PhSonySlaveController.h"

#include "GraphicSyncTestSettings.h"

namespace Ui {
class GraphicSyncTestWindow;
}

class GraphicSyncTestWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit GraphicSyncTestWindow(GraphicSyncTestSettings *settings);
	~GraphicSyncTestWindow();

private slots:
	void beforePaint(int frequency);

private:
	Ui::GraphicSyncTestWindow *ui;
	GraphicSyncTestSettings *_settings;
	PhSonySlaveController _sony;
	QTime _time;
	int _lastTime;
	int _count;
};

#endif // GRAPHICSYNCTESTWINDOW_H
