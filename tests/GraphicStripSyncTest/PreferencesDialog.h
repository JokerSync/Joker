#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

#include "GraphicStripSyncTestSettings.h"

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PreferencesDialog(GraphicStripSyncTestSettings *settings, QWidget *parent = 0);
	~PreferencesDialog();

private slots:
	void on_delaySpinBox_valueChanged(int delay);

	void on_useQuarterFrameRadioButton_toggled(bool checked);

private:
	Ui::PreferencesDialog *ui;
	GraphicStripSyncTestSettings *_settings;
};

#endif // PREFERENCESDIALOG_H
