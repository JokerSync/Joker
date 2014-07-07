/**
 * Copyright (C) 2012-2014 Phonations
 * License: http://www.gnu.org/licenses/gpl.html GPL version 2 or higher
 */

#include "PhDialogButtonBox.h"
#include "PhTools/PhDebug.h"

#include <QPushButton>
#include <QTimer>

PhDialogButtonBox::PhDialogButtonBox(QWidget *parent) :
	QDialogButtonBox(parent)
{
}

void PhDialogButtonBox::showEvent(QShowEvent *)
{
	// Unset the Cancel button from default
	QPushButton * cancelBtn = this->button(QDialogButtonBox::Cancel);
	if(cancelBtn) {
		cancelBtn->setAutoDefault(false);
		cancelBtn->setDefault(false);
		cancelBtn->setText(tr("Cancel"));
		PHDEBUG << "Set cancel";
	}

	// Set the Ok Button to default
	QPushButton * okBtn = this->button(QDialogButtonBox::Ok);
	if(okBtn) {
		okBtn->setAutoDefault(true);
		okBtn->setDefault(true);
		okBtn->setText(tr("Ok"));
		PHDEBUG << "Set Ok";
	}
}

