/**
* Copyright (C) 2012-2013 Phonations
* License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
*/

#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QListWidgetItem>

namespace Ui {
class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit PreferencesDialog(QSettings *settings, QWidget *parent = 0);
	~PreferencesDialog();

private slots:

	void on_spinBoxDelay_valueChanged(int delay);
	void on_radioButtonQF_toggled(bool checked);
	void on_sliderStripHeight_valueChanged(int position);
	void on_cBoxSonyAutoconnect_toggled(bool checked);
	void on_spinBoxSpeed_valueChanged(int speed);
	void on_listWidgetFont_itemClicked(QListWidgetItem *item);
	void on_listWidgetFont_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);


	void on_buttonBox_accepted();

	void on_buttonBox_rejected();

	void on_cBoxLastFile_toggled(bool checked);

	void on_cBoxFullscreen_toggled(bool checked);

	void on_sliderBoldness_valueChanged(int value);

	void on_lineEditFilter_textEdited(const QString &arg1);

	void on_cBoxDeinterlace_clicked();

private:
	Ui::PreferencesDialog *ui;
	QSettings *_settings;
	QString _oldFont;
	bool _oldUseQuarterFrame;
	int _oldDelay;
	int _oldSpeed;
	int _oldBolness;
	float _oldStripHeight;
	bool _oldSonyAutoConnect;
	bool _oldOpenLastFile;
	bool _oldStartFullScreen;
	bool _oldDeinterlace;
	QMap<QString, QString> fontList;

};

#endif // PREFERENCESDIALOG_H
