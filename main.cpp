#include <QApplication>
#include "gamecontroller.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    
    GameController controller;
    controller.init();
    
    return a.exec();
}