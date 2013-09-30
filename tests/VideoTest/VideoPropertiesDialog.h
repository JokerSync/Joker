#ifndef VIDEOPROPERTIESDIALOG_H
#define VIDEOPROPERTIESDIALOG_H

#include <QDialog>

namespace Ui {
class VideoPropertiesDialog;
}

class VideoPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	explicit VideoPropertiesDialog(QWidget *parent = 0);
	~VideoPropertiesDialog();

private:
	Ui::VideoPropertiesDialog *ui;
};

#endif // VIDEOPROPERTIESDIALOG_H
