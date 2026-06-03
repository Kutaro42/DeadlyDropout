#include <QApplication>
#include "appcontroller.h"

// ═══════════════════════════════════════════════════════════════════
//  main.cpp — point d'entrée de l'application Deadly Dropout.
//  Crée le GameController et lance la boucle Qt.
// ═══════════════════════════════════════════════════════════════════
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("Deadly Dropout");

    AppController appcontroller;
    //appcontroller.show();
    appcontroller.run();
    return app.exec();
}
