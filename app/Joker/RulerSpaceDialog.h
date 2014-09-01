/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef RULERSPACEDIALOG_H
#define RULERSPACEDIALOG_H

#include <QDialog>

#include "JokerSettings.h"

namespace Ui {
class RulerSpaceDialog;
}

/**
 * @brief This dialog display size setting for the ruler.
 */
class RulerSpaceDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	 * @brief RulerSpaceDialog constructor
	 * @param settings The main window settings
	 * @param parent The parent
	 */
	explicit RulerSpaceDialog(JokerSettings * settings, QWidget *parent = 0);
	~RulerSpaceDialog();

private slots:
	void on_horizontalSlider_sliderMoved(int position);

	void on_spinBox_valueChanged(int value);

	void on_buttonBox_rejected();

private:
	Ui::RulerSpaceDialog *ui;
	JokerSettings * _settings;
	int _oldSpace;
};

#endif // RULERSPACEDIALOG_H
