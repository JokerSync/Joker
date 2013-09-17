#include "PhTimeCodeDlg.h"

TimeCodeDlg::TimeCodeDlg(PhTimeCodeType tcType, PhFrame frame, QWidget *parent)
	:QDialog(parent), _tcType(tcType)
{
	QHBoxLayout *layout = new QHBoxLayout;

	QPushButton *backBtn = new QPushButton("Back", this);
	QPushButton *goToBtn = new QPushButton("Go to", this);
	textLineEdit = new QLineEdit(this);

	// Initialize the timecode text
	textLineEdit->setText(PhTimeCode::stringFromFrame(frame, _tcType));
	// Select the whole texte
	textLineEdit->selectAll();
	// Put the focus on the text field
	textLineEdit->setFocus();
	goToBtn->setDefault(true);

	layout->addWidget(textLineEdit);
	layout->addWidget(backBtn);
	layout->addWidget(goToBtn);

	connect(backBtn, SIGNAL(clicked()), this, SLOT(reject()));
	connect(goToBtn, SIGNAL(clicked()), this, SLOT(accept()));

	this->setLayout(layout);
}

PhFrame TimeCodeDlg::frame()
{
	return PhTimeCode::frameFromString(textLineEdit->text(), _tcType);
}



