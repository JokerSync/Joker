/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#ifndef PHDIALOGBUTTONBOX_H
#define PHDIALOGBUTTONBOX_H

#include <QDialogButtonBox>

class PhDialogButtonBox : public QDialogButtonBox
{
	Q_OBJECT
public:
	explicit PhDialogButtonBox(QWidget *parent = 0);

signals:

public slots:

private slots:
	void updateButtons();


};

#endif // PHDIALOGBUTTONBOX_H
