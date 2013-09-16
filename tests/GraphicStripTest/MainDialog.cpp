#include "MainDialog.h"

MainDialog::MainDialog(QWidget *parent)
	:QDialog(parent)
{
	_dlg = new Dialog(this);

	// Ce bouton nous permettra d'afficher notre boîte de dialogue
	QPushButton *modalDlgBtn = new QPushButton(" Open Modal dialog", this);

	// On demande à Qt d'executer showDialogModal() lorsque le bouton est clique
	connect(modalDlgBtn, SIGNAL(clicked()), this, SLOT(showDialogModal()));
}

void MainDialog::showDialogModal()
{
	// on affiche la boite de dialogue de facon modale
	_dlg->setModal(true);
	_dlg->show();
}
