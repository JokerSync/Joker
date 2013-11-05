/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PREFPANEL_H
#define PREFPANEL_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class PrefPanel;
}

class PrefPanel : public QDialog
{
	Q_OBJECT

public:
	explicit PrefPanel(QSettings *settings, QWidget *parent = 0);
	~PrefPanel();

private slots:

	void on_spinBoxDelay_valueChanged(int delay);
	void on_cBoxQuarterFram_toggled(bool checked);
	void on_sliderStripHeight_sliderMoved(int position);
	void on_cBoxSonyAutoconnect_toggled(bool checked);


	void on_buttonBox_accepted();

	void on_buttonBox_rejected();

private:
	Ui::PrefPanel *ui;
	QSettings *_settings;
	bool _oldUseQuarterFrame;
	int _oldDelay;
	float _oldStripHeight;
	bool _oldSonyAutoConnect;

};

#endif // PREFPANEL_H
