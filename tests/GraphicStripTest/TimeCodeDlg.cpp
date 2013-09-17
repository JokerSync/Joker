#include "TimeCodeDlg.h"

TimeCodeDlg::TimeCodeDlg(PhTimeCodeType tcType, PhFrame frame, QWidget *parent)
	:QDialog(parent), _tcType(tcType)
{
	QHBoxLayout *layout = new QHBoxLayout;

	// Elle est simplement composé d'un bouton "Fermer"
	QPushButton *backBtn = new QPushButton("Back", this);
	QPushButton *goToBtn = new QPushButton("Go to", this);
	textLineEdit = new QLineEdit(this);

	textLineEdit->setText(PhTimeCode::stringFromFrame(frame, _tcType));
	textLineEdit->selectAll();
	textLineEdit->setFocus();
	goToBtn->setDefault(true);

	layout->addWidget(textLineEdit);
	layout->addWidget(backBtn);
	layout->addWidget(goToBtn);


	// lequel ferme la fenetre lorsqu'on clic dessus
	connect(backBtn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(goToBtn, SIGNAL(clicked()), this, SLOT(accept()));

	this->setLayout(layout);
}

PhFrame TimeCodeDlg::frame()
{
	return PhTimeCode::frameFromString(textLineEdit->text(), _tcType);
}



