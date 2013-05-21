#include "videotest.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    videoTest w;
    w.show();
    
    return a.exec();
}
