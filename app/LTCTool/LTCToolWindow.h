#ifndef LTCTOOLWINDOW_H
#define LTCTOOLWINDOW_H

#include <QMainWindow>

namespace Ui {
class LTCToolWindow;
}

class LTCToolWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit LTCToolWindow(QWidget *parent = 0);
	~LTCToolWindow();

private:
	Ui::LTCToolWindow *ui;
};

#endif // LTCTOOLWINDOW_H
