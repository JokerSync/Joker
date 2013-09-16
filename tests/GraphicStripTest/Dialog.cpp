#include "Dialog.h"

Dialog::Dialog(QWidget *parent)
:QDialog(parent)
{
	QHBoxLayout *layout = new QHBoxLayout;

	// Elle est simplement composé d'un bouton "Fermer"
	QPushButton *backBtn = new QPushButton("Back", this);
	goToBtn = new QPushButton("Go to", this);
	textLineEdit = new QLineEdit(this);

	layout->addWidget(textLineEdit);
	layout->addWidget(backBtn);
	layout->addWidget(goToBtn);

	// lequel ferme la fenetre lorsqu'on clic dessus
	connect(backBtn, SIGNAL(clicked()), this, SLOT(accept()));

	this->setLayout(layout);
}



