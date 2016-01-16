/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef RULERSPACEDIALOG_H
#define RULERSPACEDIALOG_H

#include <QDialog>

#include "JokerSettings.h"

namespace Ui {
class TimeBetweenTwoFeetDialog;
}

/**
 * @brief This dialog display size setting for the ruler.
 */
class TimeBetweenTwoFeetDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * @brief TimeBetweenTwoFeetDialog constructor
	 * @param settings The main window settings
	 * @param parent The parent
	 */
	explicit TimeBetweenTwoFeetDialog(JokerSettings * settings, QWidget *parent = 0);
	~TimeBetweenTwoFeetDialog();

private slots:
	void on_horizontalSlider_sliderMoved(int position);

	void on_spinBox_valueChanged(int value);

	void on_buttonBox_rejected();

private:
	Ui::TimeBetweenTwoFeetDialog *ui;
	JokerSettings * _settings;
	int _oldTimeBetweenTwoFeet;
};

#endif // RULERSPACEDIALOG_H
