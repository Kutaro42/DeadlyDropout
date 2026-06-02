#include "appcontroller.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    appController w;
    w.run();
    return a.exec();
}
