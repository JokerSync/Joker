#ifndef GRAPHICSYNCTESTWINDOW_H
#define GRAPHICSYNCTESTWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "PhSync/PhSonySlaveController.h"

namespace Ui {
class GraphicSyncTestWindow;
}

class GraphicSyncTestWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit GraphicSyncTestWindow(QWidget *parent = 0);
	~GraphicSyncTestWindow();

private slots:
	void beforePaint(int frequency);

private:
	Ui::GraphicSyncTestWindow *ui;
	QSettings _settings;
	PhSonySlaveController _sony;
};

#endif // GRAPHICSYNCTESTWINDOW_H
