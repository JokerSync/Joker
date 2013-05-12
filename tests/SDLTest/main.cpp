#include "dwwindow.h"
#include <QApplication>

using namespace std;

int main( int argc, char* argv[] )
{
    QApplication a(argc, argv);
    DWwindow w(400, 300);

    w.show();

    return a.exec();
}
